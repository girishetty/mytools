/*
 * Copyright 2017 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <locale>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <libxml/xmlreader.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "mpd_parser.h"

using std::cout;

/*
======================================================================================================
======================================================================================================
*/

// A macro to convert a char* to xmlChar*
#define TO_XMLCHAR(x) reinterpret_cast<const xmlChar*>(x)
// A macro to convert a xmlChar* to char*
#define FROM_XMLCHAR(x) reinterpret_cast<const char*>(x)

int64_t ParseXsDateTime(const std::string& value) {
  return strtoll(value.c_str(), NULL, 10);
}

int64_t ParseXsDuration(const std::string& value) {
  if (value[0] != 'P') {
    return -1;
  }

  // Build scan pattern
  size_t yPos = value.find('Y', 0);
  size_t dPos = value.find('D', 0);
  size_t hPos = value.find('H', 0);
  size_t sPos = value.find('S', 0);

  size_t minPos;
  size_t monthPos;
  size_t tPos = value.find('T', 0);
  if (tPos != std::string::npos) {
    std::string date_component = value.substr(0, tPos);
    std::string time_component = value.substr(tPos);
    monthPos = date_component.find('M', 0);
    minPos = time_component.find('M', 0);
  } else {
    minPos = std::string::npos;
    monthPos = value.find('M', 0);
  }

  int num_args = 0;
  std::string pattern = "P";
  std::map<int, float> multiplier;

  if (yPos != std::string::npos) {
    pattern.append("%lfY");
    multiplier[num_args] = 31556926;
    num_args++;
  }
  if (monthPos != std::string::npos) {
    pattern.append("%lfM");
    multiplier[num_args] = 2629743.83;
    num_args++;
  }
  if (dPos != std::string::npos) {
    pattern.append("%lfD");
    multiplier[num_args] = 86400;
    num_args++;
  }
  if (hPos != std::string::npos || minPos != std::string::npos ||
      sPos != std::string::npos) {
    pattern.append("T");
  }
  if (hPos != std::string::npos) {
    pattern.append("%lfH");
    multiplier[num_args] = 3600;
    num_args++;
  }
  if (minPos != std::string::npos) {
    pattern.append("%lfM");
    multiplier[num_args] = 60;
    num_args++;
  }
  if (sPos != std::string::npos) {
    pattern.append("%lfS");
    multiplier[num_args] = 1;
    num_args++;
  }

  int num_matched = 0;
  double v[6];
  switch (num_args) {
    case 1:
      num_matched = sscanf(value.c_str(), pattern.c_str(), &v[0]);
      break;
    case 2:
      num_matched = sscanf(value.c_str(), pattern.c_str(), &v[0], &v[1]);
      break;
    case 3:
      num_matched = sscanf(value.c_str(), pattern.c_str(), &v[0], &v[1], &v[2]);
      break;
    case 4:
      num_matched =
          sscanf(value.c_str(), pattern.c_str(), &v[0], &v[1], &v[2], &v[3]);
      break;
    case 5:
      num_matched = sscanf(value.c_str(), pattern.c_str(), &v[0], &v[1], &v[2],
                           &v[3], &v[4]);
      break;
    case 6:
      num_matched = sscanf(value.c_str(), pattern.c_str(), &v[0], &v[1], &v[2],
                           &v[3], &v[4], &v[5]);
      break;
    default:
      return -1;
  }

  if (num_args != num_matched) {
    return -1;
  }

  // TODO(rmrossi): According to ISO8601 spec, only the last component may have
  // a fraction. Add a check here and fail due to parsing error if that is
  // not the case.

  int64_t milliseconds = 0;
  for (int i = 0; i < num_args; i++) {
    milliseconds += (v[i] * multiplier[i] * 1000);
  }

  return milliseconds;
}

  enum PartIndices {
    // An index into an array returned by getUriIndices.
    // The value at this position in the array is the index of the ':' after
    // the scheme. Equals -1 if the URI is a relative reference (no scheme).
    // The hier-part starts at (schemeColon + 1), including when the URI has no
    // scheme.
    SCHEME_COLON = 0,
    // An index into an array returned by getUriIndices.
    // The value at this position in the array is the index of the path part.
    // Equals (schemeColon + 1) if no authority part, (schemeColon + 3) if the
    // authority part consists of just "//", and (query) if no path part. The
    // characters starting at this index can be "//" only if the authority part
    // is non-empty (in this case the double-slash means the first segment is
    // empty).
    PATH = 1,
    // An index into an array returned by getUriIndices.
    // The value at this position in the array is the index of the query part,
    // including the '?' before the query. Equals fragment if no query part,
    // and (fragment - 1) if the query part is a single '?' with no data.
    QUERY = 2,
    // An index into an array returned by getUriIndices.
    // The value at this position in the array is the index of the fragment
    // part, including the '#' before the fragment. Equal to the length of the
    // URI if no fragment part, and (length - 1) if the fragment part is a
    // single '#' with no data.
    FRAGMENT = 3,
    // The length of arrays returned by getUriIndices.
    INDEX_COUNT = 4
  };

std::unique_ptr<int[]> GetUriIndices(const std::string& uri_string) {
  std::unique_ptr<int[]> indices(new int[INDEX_COUNT]);

  indices[SCHEME_COLON] = 0;
  indices[PATH] = 0;
  indices[QUERY] = 0;
  indices[FRAGMENT] = 0;

  int length = uri_string.length();
  if (length == 0 || uri_string.c_str() == nullptr) {
    indices[SCHEME_COLON] = -1;
    return indices;
  }

  // Determine outer structure from right to left.
  // Uri = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
  int fragment_index = uri_string.find('#');
  if (fragment_index == -1) {
    fragment_index = length;
  }
  int query_index = uri_string.find('?');
  if (query_index == -1 || query_index > fragment_index) {
    // '#' before '?': '?' is within the fragment.
    query_index = fragment_index;
  }
  // Slashes are allowed only in hier-part so any colon after the first slash
  // is part of the
  // hier-part, not the scheme colon separator.
  int scheme_index_limit = uri_string.find('/');
  if (scheme_index_limit == -1 || scheme_index_limit > query_index) {
    scheme_index_limit = query_index;
  }
  int scheme_index = uri_string.find(':');
  if (scheme_index > scheme_index_limit) {
    // '/' before ':'
    scheme_index = -1;
  }

  // Determine hier-part structure: hier-part = "//" authority path / path
  // This block can also cope with schemeIndex == -1.
  bool has_authority = scheme_index + 2 < query_index &&
                       uri_string[scheme_index + 1] == '/' &&
                       uri_string[scheme_index + 2] == '/';
  int path_index;
  if (has_authority) {
    // find first '/' after "://"
    path_index = uri_string.find("/", scheme_index + 3);
    if (path_index == -1 || path_index > query_index) {
      path_index = query_index;
    }
  } else {
    path_index = scheme_index + 1;
  }

  indices[SCHEME_COLON] = scheme_index;
  indices[PATH] = path_index;
  indices[QUERY] = query_index;
  indices[FRAGMENT] = fragment_index;
  return indices;
}

std::string RemoveDotSegments(std::string uri, int offset, int limit) {
  if (offset >= limit) {
    // Nothing to do.
    return uri;
  }
  if (uri[offset] == '/') {
    // If the path starts with a /, always retain it.
    offset++;
  }
  // The first character of the current path segment.
  int segment_start = offset;
  int i = offset;
  while (i <= limit) {
    int next_segment_start = -1;
    if (i == limit) {
      next_segment_start = i;
    } else if (uri[i] == '/') {
      next_segment_start = i + 1;
    } else {
      i++;
      continue;
    }
    // We've encountered the end of a segment or the end of the path. If the
    // final segment was "." or "..", remove the appropriate segments of the
    // path.
    if (i == segment_start + 1 && uri[segment_start] == '.') {
      // Given "abc/def/./ghi", remove "./" to get "abc/def/ghi".
      uri.erase(segment_start, next_segment_start - segment_start);
      limit -= next_segment_start - segment_start;
      i = segment_start;
    } else if (i == segment_start + 2 && uri[segment_start] == '.' &&
               uri[segment_start + 1] == '.') {
      // Given "abc/def/../ghi", remove "def/../" to get "abc/ghi".
      std::string prev = uri.substr(0, segment_start - 2);
      int prev_segment_start = prev.find_last_of("/") + 1;
      int removeFrom =
          prev_segment_start > offset ? prev_segment_start : offset;
      uri.erase(removeFrom, next_segment_start - removeFrom);
      limit -= next_segment_start - removeFrom;
      segment_start = prev_segment_start;
      i = prev_segment_start;
    } else {
      i++;
      segment_start = i;
    }
  }
  return uri;
}
std::string Resolve(const std::string& base_uri,
                             const std::string& reference_uri) {
  std::string uri;

  std::unique_ptr<int[]> ref_indices = GetUriIndices(reference_uri);
  if (ref_indices[SCHEME_COLON] != -1) {
    // The reference is absolute. The target Uri is the reference.
    uri.append(reference_uri);
    RemoveDotSegments(uri, ref_indices[PATH], ref_indices[QUERY]);
    return uri;
  }

  std::unique_ptr<int[]> base_indices = GetUriIndices(base_uri);
  if (ref_indices[FRAGMENT] == 0) {
    // The reference is empty or contains just the fragment part, then the
    // target Uri is the concatenation of the base Uri without its fragment,
    // and the reference.
    std::string no_frag = base_uri.substr(0, base_indices[FRAGMENT]);
    uri.append(no_frag);
    uri.append(reference_uri);
    return uri;
  }

  if (ref_indices[QUERY] == 0) {
    // The reference starts with the query part. The target is the base up to
    // (but excluding) the query, plus the reference.
    std::string no_query = base_uri.substr(0, base_indices[QUERY]);
    uri.append(no_query);
    uri.append(reference_uri);
    return uri;
  }

  if (ref_indices[PATH] != 0) {
    // The reference has authority. The target is the base scheme plus the
    // reference.
    int base_limit = base_indices[SCHEME_COLON] + 1;
    std::string base = base_uri.substr(0, base_limit);
    uri.append(base);
    uri.append(reference_uri);
    return RemoveDotSegments(uri, base_limit + ref_indices[PATH],
                             base_limit + ref_indices[QUERY]);
  }

  if (ref_indices[PATH] != ref_indices[QUERY] &&
      reference_uri[ref_indices[PATH]] == '/') {
    // The reference path is rooted. The target is the base scheme and
    // authority (if any), plus the reference.
    std::string base = base_uri.substr(0, base_indices[PATH]);
    uri.append(base);
    uri.append(reference_uri);
    return RemoveDotSegments(uri, base_indices[PATH],
                             base_indices[PATH] + ref_indices[QUERY]);
  }

  // The target Uri is the concatenation of the base Uri up to (but excluding)
  // the last segment, and the reference. This can be split into 2 cases:
  if (base_indices[SCHEME_COLON] + 2 < base_indices[PATH] &&
      base_indices[PATH] == base_indices[QUERY]) {
    // Case 1: The base hier-part is just the authority, with an empty path. An
    // additional '/' is needed after the authority, before appending the
    // reference.
    std::string base = base_uri.substr(0, base_indices[PATH]);
    uri.append(base).append("/").append(reference_uri);
    return RemoveDotSegments(uri, base_indices[PATH],
                             base_indices[PATH] + ref_indices[QUERY] + 1);
  } else {
    // Case 2: Otherwise, find the last '/' in the base hier-part and append
    // the reference after it. If base hier-part has no '/', it could only mean
    // that it is completely empty or contains only one segment, in which case
    // the whole hier-part is excluded and the reference is appended right
    // after the base scheme colon without an added '/'.
    std::string query = base_uri.substr(0, base_indices[QUERY]);
    int lastSlashIndex = query.find_last_of('/');
    int baseLimit =
        lastSlashIndex == -1 ? base_indices[PATH] : lastSlashIndex + 1;
    std::string base = base_uri.substr(0, baseLimit);
    uri.append(base);
    uri.append(reference_uri);
    return RemoveDotSegments(uri, base_indices[PATH],
                             baseLimit + ref_indices[QUERY]);
  }
}

/*
======================================================================================================
======================================================================================================
*/

MediaPresentationDescriptionParser::MediaPresentationDescriptionParser(
    std::string content_id)
    : content_id_(content_id) {}

void MediaPresentationDescriptionParser::Parse(const std::string& xml, const std::string& connection_url) const {
  std::unique_ptr<xmlTextReader, void (*)(xmlTextReaderPtr)> reader(
      xmlReaderForMemory(xml.data(), xml.length(), connection_url.c_str(),
                         nullptr, 0),
      xmlFreeTextReader);

  int ret = xmlTextReaderRead(reader.get());
  if (ret == 1) {
    // Read through the XML skipping comments nodes
    while (CurrentNodeNameEquals(reader.get(), "#comment")) {
      ret = xmlTextReaderRead(reader.get());
      if (ret != 1) {
        break;
      }
      xmlNodePtr commentNode = xmlTextReaderCurrentNode(reader.get());
      // Ignore this comment block
      (void) commentNode;
    }
    if (CurrentNodeNameEquals(reader.get(), "MPD")) {
      ParseMediaPresentationDescription(reader.get(), connection_url);
    } else {
      cout << "Can't find MPD TAG/Node in this manifest." << std::endl;
    }
  } else {
    cout << "Failed to open MPD manifest" << std::endl;
  }
}

void MediaPresentationDescriptionParser::ParseMediaPresentationDescription(
    xmlTextReaderPtr reader,
    const std::string& base_url) const {
  xmlNodePtr node = xmlTextReaderCurrentNode(reader);

  int64_t availabilityStartTime =
      ParseDateTime(node, "availabilityStartTime", -1);
  int64_t duration_ms = ParseDuration(node, "mediaPresentationDuration", -1);
  int64_t minBufferTimeMs = ParseDuration(node, "minBufferTime", -1);
  xmlChar* type_string = xmlGetProp(node, TO_XMLCHAR("type"));
  bool dynamic = false;
  if (type_string != nullptr) {
    dynamic =
        xmlStrEqual(TO_XMLCHAR("dynamic"), type_string) == 1 ? true : false;
  }
  xmlFree(type_string);

  int64_t min_update_time_ms =
      (dynamic) ? ParseDuration(node, "minimumUpdatePeriod", -1) : -1;
  int64_t time_shift_buffer_depth_ms =
      (dynamic) ? ParseDuration(node, "timeShiftBufferDepth", -1) : -1;
  std::string location;

  int64_t next_period_start_ms = dynamic ? -1 : 0;
  bool seen_early_access_period = false;
  bool seen_first_base_Url = false;
  std::string base_url_override(base_url);

  int ret;
  int parent_depth = xmlTextReaderDepth(reader);
  int depth;
  do {
    ret = xmlTextReaderRead(reader);
    if (ret != 1) {
      break;
    }
    depth = xmlTextReaderDepth(reader);
    xmlNodePtr child = xmlTextReaderCurrentNode(reader);
    cout << "GIRISH: MediaPresentationDescriptionParser::ParseMediaPresentationDescription:" <<
            " name: " << child->name << " depth: " << depth << " line: " << xmlGetLineNo(child) << std::endl;
    if (CurrentNodeNameEquals(reader, "BaseURL")) {
      if (!seen_first_base_Url) {
        base_url_override = ParseBaseUrl(reader, base_url_override);
        seen_first_base_Url = true;
      }
    } else if (CurrentNodeNameEquals(reader, "SupplementalProperty")) {
      ParseDescriptorType(child);
      NextSibling(reader);
    } else if (CurrentNodeNameEquals(reader, "EssentialProperty")) {
      ParseDescriptorType(child);
      NextSibling(reader);
    } else if (CurrentNodeNameEquals(reader, "UTCTiming")) {
      ParseDescriptorType(child);
      NextSibling(reader);
    } else if (CurrentNodeNameEquals(reader, "Location")) {
      location = NextText(reader);
    } else if (CurrentNodeNameEquals(reader, "Period") &&
               !seen_early_access_period) {
      ParsePeriod(reader, base_url_override, next_period_start_ms);
    }
  } while (depth > parent_depth);

  if (duration_ms == -1) {
    if (next_period_start_ms != -1) {
      // If we know the end time of the final period, we can use it as the
      // duration.
      duration_ms = next_period_start_ms;
    } else if (!dynamic) {
      return;
    }
  }
}

void MediaPresentationDescriptionParser::ParseDescriptorType(xmlNodePtr node) const {
  xmlChar* scheme_prop = xmlGetProp(node, TO_XMLCHAR("schemeIdUri"));
  xmlChar* value_prop = xmlGetProp(node, TO_XMLCHAR("value"));
  xmlChar* id_prop = xmlGetProp(node, TO_XMLCHAR("id"));
  std::string scheme_id;
  if (scheme_prop != nullptr) {
    scheme_id = std::string(FROM_XMLCHAR(scheme_prop));
    xmlFree(scheme_prop);
  }
  std::string value;
  if (value_prop != nullptr) {
    value = std::string(FROM_XMLCHAR(value_prop));
    xmlFree(value_prop);
  }
  std::string id;
  if (id_prop != nullptr) {
    id = std::string(FROM_XMLCHAR(id_prop));
    xmlFree(id_prop);
  }
  cout << "scheme_id: " << scheme_id << " value: " << value << " id: " << id << std::endl;
}

void MediaPresentationDescriptionParser::ParsePeriod(xmlTextReaderPtr reader,
                                                const std::string& base_url,
                                                int64_t defaultStartMs) const {
  xmlNodePtr node = xmlTextReaderCurrentNode(reader);
  std::string id = GetAttributeValue(node, "id");
  int64_t start_ms = ParseDuration(node, "start", defaultStartMs);
  int64_t duration_ms = ParseDuration(node, "duration", -1);
  std::string base_url_override(base_url);
  bool seen_first_base_url = false;
  int ret;
  int parent_depth = xmlTextReaderDepth(reader);
  int depth;
  if (xmlTextReaderIsEmptyElement(reader)) {
    const char* name = (const char*) xmlTextReaderConstName(reader);
    if (name == nullptr) {
      name = "none";
    }
    cout << __func__ << " Emptry Element: " << name << std::endl;
  } else
  do {
    ret = xmlTextReaderRead(reader);
    if (ret != 1) {
      break;
    }
    depth = xmlTextReaderDepth(reader);
    xmlNodePtr child = xmlTextReaderCurrentNode(reader);
    cout << "GIRISH: MediaPresentationDescriptionParser::ParsePeriod:" <<
            " name: " << child->name << " depth: " << depth << " line: " << xmlGetLineNo(child) << std::endl;
    if (NodeNameEquals(child, "BaseURL")) {
      if (!seen_first_base_url) {
        base_url_override = ParseBaseUrl(reader, base_url_override);
        seen_first_base_url = true;
      }
    } else if (CurrentNodeNameEquals(reader, "SupplementalProperty")) {
      ParseDescriptorType(child);
      NextSibling(reader);
    } else if (NodeNameEquals(child, "AdaptationSet")) {
      ParseAdaptationSet(reader, base_url_override);
    } else if (NodeNameEquals(child, "SegmentBase")) {
      ParseSegmentBase(reader, base_url_override);
    } else if (NodeNameEquals(child, "SegmentList")) {
      ParseSegmentList(reader, base_url_override);
    } else if (NodeNameEquals(child, "SegmentTemplate")) {
      ParseSegmentTemplate(reader, base_url_override);
    }
  } while (depth > parent_depth);

}

// AdaptationSet parsing.

void MediaPresentationDescriptionParser::ParseAdaptationSet(
    xmlTextReaderPtr reader,
    const std::string& base_url) const {
  xmlNodePtr node = xmlTextReaderCurrentNode(reader);
  int32_t id;
  if (!ParseInt(node, "id", &id, -1)) {
    cout << "Failed to parse adaptation set 'id' attribute" << std::endl;
    return;
  }
  AdaptationType content_type = ParseContentType(node);

  std::string mime_type = GetAttributeValue(node, "mimeType");
  std::string codecs = GetAttributeValue(node, "codecs");
  int32_t width;
  int32_t height;
  if (!ParseInt(node, "width", &width, -1)) {
    cout << "Failed to parse adaptation set 'width' attribute" << std::endl;
    return;
  }
  if (!ParseInt(node, "height", &height, -1)) {
    cout << "Failed to parse adaptation set 'height' attribute" << std::endl;
    return;
  }
  double frame_rate;
  if (!ParseFrameRate(node, &frame_rate, -1)) {
    return;
  }
  int32_t max_playout_rate;
  if (!ParseInt(node, "maxPlayoutRate", &max_playout_rate, 1)) {
    return;
  }
  int32_t audio_channels = -1;
  int32_t audio_sampling_rate;
  if (!ParseInt(node, "audioSamplingRate", &audio_sampling_rate, -1)) {
    cout << "Failed to parse adaptation set 'audioSamplingRate' attribute" << std::endl;
    return;
  }
  std::string language = GetAttributeValue(node, "lang");
  std::string base_url_override(base_url);
  bool seen_first_base_Url = false;
  int ret;
  int parent_depth = xmlTextReaderDepth(reader);
  int depth;
  if (xmlTextReaderIsEmptyElement(reader)) {
    const char* name = (const char*) xmlTextReaderConstName(reader);
    if (name == nullptr) {
      name = "none";
    }
    cout << __func__ << " Emptry Element: " << name << std::endl;
  } else
  do {
    // Read the next node
    ret = xmlTextReaderRead(reader);
    if (ret != 1) {
      break;
    }
    depth = xmlTextReaderDepth(reader);
    xmlNodePtr child = xmlTextReaderCurrentNode(reader);
    cout << "GIRISH: MediaPresentationDescriptionParser::ParseAdaptationSet:" <<
                 " name: " << child->name << " depth: " << depth << " line: " << xmlGetLineNo(child) << std::endl;
    if (NodeNameEquals(child, "BaseURL")) {
      if (!seen_first_base_Url) {
        base_url_override = ParseBaseUrl(reader, base_url_override);
        seen_first_base_Url = true;
      }
    } else if (CurrentNodeNameEquals(reader, "Accessibility")) {
      ParseDescriptorType(child);
      NextSibling(reader);
    } else if (CurrentNodeNameEquals(reader, "SupplementalProperty")) {
      ParseDescriptorType(child);
      NextSibling(reader);
    } else if (CurrentNodeNameEquals(reader, "EssentialProperty")) {
      ParseDescriptorType(child);
      NextSibling(reader);
    } else if (NodeNameEquals(child, "ContentProtection")) {
      ParseContentProtection(reader);
    } else if (NodeNameEquals(child, "ContentComponent")) {
      std::string child_lang = GetAttributeValue(child, "lang");
      language = CheckLanguageConsistency(language, child_lang);
      content_type =
          CheckContentTypeConsistency(content_type, ParseContentType(child));
      NextSibling(reader);
    } else if (NodeNameEquals(child, "Representation")) {
      ParseRepresentation(
          reader, base_url_override, mime_type, codecs, width, height,
          frame_rate, max_playout_rate, audio_channels, audio_sampling_rate,
          language);
      content_type = CheckContentTypeConsistency(
          content_type, GetContentType(mime_type));
    } else if (NodeNameEquals(child, "AudioChannelConfiguration")) {
      audio_channels = ParseAudioChannelConfiguration(reader);
      NextSibling(reader);
    } else if (NodeNameEquals(child, "SegmentBase")) {
      ParseSegmentBase(reader, base_url_override);
    } else if (NodeNameEquals(child, "SegmentList")) {
      ParseSegmentList(reader, base_url_override);
    } else if (NodeNameEquals(child, "SegmentTemplate")) {
      ParseSegmentTemplate(reader, base_url_override);
    }
  } while (depth > parent_depth);
}

const char kBaseTypeVideo[] = "video";
const char kBaseTypeAudio[] = "audio";
const char kBaseTypeText[] = "text";
const char kBaseTypeApplication[] = "application";

const char kApplicationTtml[] = "application/ttml+xml";
const char kApplicationMp4[] = "application/mp4";

const char kVideoUnknown[] = "video/x-unknown";
const char kVideoMP4[] = "video/mp4";
const char kVideoWebM[] = "video/webm";
const char kVideoH263[] = "video/3gpp";
const char kVideoH264[] = "video/avc";
const char kVideoH265[] = "video/hevc";
const char kVideoVP8[] = "video/x-vnd.on2.vp8";
const char kVideoVP9[] = "video/x-vnd.on2.vp9";
const char kVideoMP4V[] = "video/mp4v-es";
const char kVideoMPEG2[] = "video/mpeg2";
const char kVideoVC1[] = "video/wvc1";

const char kAudioUnknown[] = "audio/x-unknown";
const char kAudioMP4[] = "audio/mp4";
const char kAudioAAC[] = "audio/mp4a-latm";
const char kAudioWebM[] = "audio/webm";
const char kAudioMPEG[] = "audio/mpeg";
const char kAudioMPEG_L1[] = "audio/mpeg-L1";
const char kAudioMPEG_L2[] = "audio/mpeg-L2";
const char kAudioRaw[] = "audio/raw";
const char kAudioAC3[] = "audio/ac3";
const char kAudioE_AC3[] = "audio/eac3";
const char kAudioTrueHD[] = "audio/true-hd";
const char kAudioDTS[] = "audio/vnd.dts";
const char kAudioDTS_HD[] = "audio/vnd.dts.hd";
const char kAudioDTS_Express[] = "audio/vnd.dts.hd;profile=lbr";
const char kAudioVorbis[] = "audio/vorbis";
const char kAudioOpus[] = "audio/opus";
const char kAudioAMR_NB[] = "audio/3gpp";
const char kAudioAMR_WB[] = "audio/amr-wb";
const char kAudioFLAC[] = "audio/x-flac";

const char kTextUnknown[] = "text/x-unknown";
const char kTextVTT[] = "text/vtt";

const char kApplicationMP4[] = "application/mp4";
const char kApplicationWebM[] = "application/webm";
const char kApplicationID3[] = "application/id3";
const char kApplicationEIA608[] = "application/eia-608";
const char kApplicationSubRip[] = "application/x-subrip";
const char kApplicationTTML[] = "application/ttml+xml";
const char kApplicationM3U8[] = "application/x-mpegURL";
const char kApplicationTX3G[] = "application/x-quicktime-tx3g";
const char kApplicationMP4VTT[] = "application/x-mp4vtt";
const char kApplicationVOBsub[] = "application/vobsub";
const char kApplicationPGS[] = "application/pgs";
const char kApplicationRAWCC[] = "application/x-rawcc";

AdaptationType MediaPresentationDescriptionParser::ParseContentType(
    xmlNodePtr node) const {
  std::string content_type = GetAttributeValue(node, "contentType");
  if (content_type.empty()) {
    return AdaptationType::UNKNOWN;
  } else {
    if (kBaseTypeAudio == content_type) {
      return AdaptationType::AUDIO;
    } else if (kBaseTypeVideo == content_type) {
      return AdaptationType::VIDEO;
    } else if (kBaseTypeText == content_type) {
      return AdaptationType::TEXT;
    }
    return AdaptationType::UNKNOWN;
  }
}

bool IsText(const std::string& mime_type) {
  return mime_type == kApplicationRAWCC || mime_type == kTextVTT ||
         mime_type == kApplicationTTML;
}

bool GetTopLevelType(const std::string& mime_type, std::string& output) {
  size_t index_of_slash = mime_type.find('/');
  if (index_of_slash != std::string::npos) {
    output.assign(mime_type.c_str(), index_of_slash);
    return true;
  }
  return false;
}

bool IsAudio(const std::string& mime_type) {
  std::string top_level;
  if (GetTopLevelType(mime_type, top_level)) {
    return top_level == kBaseTypeAudio;
  }
  return false;
}

bool IsVideo(const std::string& mime_type) {
  std::string top_level;
  if (GetTopLevelType(mime_type, top_level)) {
    return top_level == kBaseTypeVideo;
  }
  return false;
}

AdaptationType MediaPresentationDescriptionParser::GetContentType(const std::string& mime_type) {
  if (mime_type.empty()) {
    return AdaptationType::UNKNOWN;
  } else if (IsVideo(mime_type)) {
    return AdaptationType::VIDEO;
  } else if (IsAudio(mime_type)) {
    return AdaptationType::AUDIO;
  } else if (IsText(mime_type)) {
    return AdaptationType::TEXT;
  }
  return AdaptationType::UNKNOWN;
}

void MediaPresentationDescriptionParser::ParseContentProtection(
    xmlTextReaderPtr reader) const {
  xmlNodePtr node = xmlTextReaderCurrentNode(reader);
  std::string scheme_id_uri = GetAttributeValue(node, "schemeIdUri");
  bool seen_pssh_element = false;
  int ret;
  int parent_depth = xmlTextReaderDepth(reader);

  int depth;
  if (xmlTextReaderIsEmptyElement(reader)) {
    const char* name = (const char*) xmlTextReaderConstName(reader);
    if (name == nullptr) {
      name = "none";
    }
    cout << __func__ << " Emptry Element: " << name << std::endl;
  } else
  do {
    ret = xmlTextReaderRead(reader);
    if (ret != 1) {
      break;
    }
    depth = xmlTextReaderDepth(reader);
    xmlNodePtr child = xmlTextReaderCurrentNode(reader);
    cout << "GIRISH: MediaPresentationDescriptionParser::ParseContentProtection:" <<
                 " name: " << child->name << " depth: " << depth << " line: " << xmlGetLineNo(child) << std::endl;
    // The cenc:pssh element is defined in 23001-7:2015.
    if (NodeNameEquals(child, "cenc:pssh")) {
      seen_pssh_element = true;
      std::string base_64 = NextText(reader);
      cout << "PSSH: " << base_64 << std::endl;
    }
  } while (depth > parent_depth);
}

// Representation parsing.
void MediaPresentationDescriptionParser::ParseRepresentation(
    xmlTextReaderPtr reader,
    const std::string& base_url,
    const std::string& adaptation_set_mime_type,
    const std::string& adaptation_set_codecs,
    int32_t adaptation_set_width,
    int32_t adaptation_set_height,
    float adaptation_set_frame_rate,
    int32_t adaptation_set_max_playout_rate,
    int32_t adaptation_set_audio_channels,
    int32_t adaptation_set_audio_samplingRate,
    const std::string& adaptation_set_language) const {

  xmlNodePtr node = xmlTextReaderCurrentNode(reader);

  // RangedUris constructed under this representation want to share memory
  // for a common base url. Take a copy and ownership will go with the
  // segment base.

  std::string id = GetAttributeValue(node, "id");
  int32_t bandwidth;
  if (!ParseInt(node, "bandwidth", &bandwidth)) {
    cout << "Failed to parse representation 'bandwidth' attribute" << std::endl;
    return;
  }

  std::string mime_type =
      GetAttributeValue(node, "mimeType", adaptation_set_mime_type);
  std::string codecs = GetAttributeValue(node, "codecs", adaptation_set_codecs);

  int32_t width;
  int32_t height;
  if (!ParseInt(node, "width", &width, adaptation_set_width)) {
    cout << "Failed to parse representation 'width' attribute" << std::endl;
    return;
  }
  if (!ParseInt(node, "height", &height, adaptation_set_height)) {
    cout << "Failed to parse representation 'height' attribute" << std::endl;
    return;
  }
  double frame_rate;
  if (!ParseFrameRate(node, &frame_rate, adaptation_set_frame_rate)) {
    cout << "Failed to parse representation 'frameRate' attribute" << std::endl;
    return;
  }
  int32_t max_playout_rate;
  if (!ParseInt(node, "maxPlayoutRate", &max_playout_rate,
                adaptation_set_max_playout_rate)) {
    cout << "Failed to parse representation 'maxPlayoutRate' attribute" << std::endl;
    return;
  }
  int32_t audio_channels = adaptation_set_audio_channels;
  int32_t audio_sampling_rate;
  if (!ParseInt(node, "audioSamplingRate", &audio_sampling_rate,
                adaptation_set_audio_samplingRate)) {
    cout << "Failed to parse representation 'audioSamplingRate' attribute" << std::endl;
    return;
  }
  std::string language = adaptation_set_language;
  cout << "GIRISH: MediaPresentationDescriptionParser::ParseRepresentation:" <<
               " name: " << node->name << " id: " << id <<
               " mime: " << mime_type << " codecs: " << codecs << " line: " << xmlGetLineNo(node) << std::endl;
  std::string base_url_override(base_url);
  bool seen_first_base_Url = false;
  int ret;
  int parent_depth = xmlTextReaderDepth(reader);
  int depth;
  if (xmlTextReaderIsEmptyElement(reader)) {
    const char* name = (const char*) xmlTextReaderConstName(reader);
    if (name == nullptr) {
      name = "none";
    }
    cout << __func__ << " Emptry Element: " << name << std::endl;
  } else
  do {
    ret = xmlTextReaderRead(reader);
    if (ret != 1) {
      break;
    }
    depth = xmlTextReaderDepth(reader);
    xmlNodePtr child = xmlTextReaderCurrentNode(reader);
    cout << "GIRISH: MediaPresentationDescriptionParser::ParseRepresentation:" <<
                 " name: " << child->name << " depth: " << depth << " line: " << xmlGetLineNo(child) << std::endl;
    if (NodeNameEquals(child, "BaseURL")) {
      if (!seen_first_base_Url) {
        base_url_override = ParseBaseUrl(reader, base_url_override);
        seen_first_base_Url = true;
      }
    } else if (CurrentNodeNameEquals(reader, "SupplementalProperty")) {
      ParseDescriptorType(child);
      NextSibling(reader);
    } else if (CurrentNodeNameEquals(reader, "EssentialProperty")) {
      ParseDescriptorType(child);
      NextSibling(reader);
    } else if (NodeNameEquals(child, "AudioChannelConfiguration")) {
      audio_channels = ParseAudioChannelConfiguration(reader);
      NextSibling(reader);
    } else if (NodeNameEquals(child, "SegmentBase")) {
      ParseSegmentBase(reader, base_url_override);
    } else if (NodeNameEquals(child, "SegmentList")) {
      ParseSegmentList(reader, base_url_override);
    } else if (NodeNameEquals(child, "SegmentTemplate")) {
      ParseSegmentTemplate(reader, base_url_override);
    } else if (NodeNameEquals(child, "ContentProtection")) {
      ParseContentProtection(reader);
    }
  } while (depth > parent_depth);
}

// SegmentBase, SegmentList and SegmentTemplate parsing.
void MediaPresentationDescriptionParser::ParseSegmentBase(
    xmlTextReaderPtr reader,
    const std::string& base_url) const {
  std::unique_ptr<std::string> new_base_url =
      std::unique_ptr<std::string>(new std::string(base_url));

  xmlNodePtr node = xmlTextReaderCurrentNode(reader);
  int64_t timescale;
  int64_t presentation_time_offset;
  if (!ParseLong(node, "timescale", &timescale, 1)) {
    cout << "Failed to parse segment base 'timescale' attribute" << std::endl;
    return;
  }
  if (!ParseLong(node, "presentationTimeOffset", &presentation_time_offset, 0)) {
    cout << "Failed to parse segment base 'presentationTimeOffset' attribute" << std::endl;
    return;
  }

  int64_t index_start = 0;
  int64_t index_length = -1;
  std::string index_range_text = GetAttributeValue(node, "indexRange");
  if (!index_range_text.empty() &&
      !ParseRange(index_range_text, &index_start, &index_length)) {
    cout << "Failed to parse segment base 'range' attribute" << std::endl;
    return;
  }

  int ret = -1;
  int parent_depth = xmlTextReaderDepth(reader);
  int depth = -1;
  if (xmlTextReaderIsEmptyElement(reader)) {
    const char* name = (const char*) xmlTextReaderConstName(reader);
    if (name == nullptr) {
      name = "none";
    }
    cout << __func__ << " Emptry Element: " << name << std::endl;
  } else
  do {
    ret = xmlTextReaderRead(reader);
    if (ret != 1) {
      break;
    }
    depth = xmlTextReaderDepth(reader);
    xmlNodePtr child = xmlTextReaderCurrentNode(reader);
    if (NodeNameEquals(child, "Initialization")) {
      // Ownership of base url for ranged uri's goes with the segment base
      ParseInitialization(reader, new_base_url.get());
      NextSibling(reader);
    }
  } while (depth > parent_depth);
}

bool MediaPresentationDescriptionParser::ParseRange(
    const std::string& range_str,
    int64_t* index_start,
    int64_t* index_length) const {
  const char* range_text = range_str.c_str();
  char* endptr = nullptr;
  *index_start = strtol(range_text, &endptr, 10);
  if (*index_start < 0 || *endptr != '-') {
    cout << "Invalid index range: invalid start" << std::endl;
    return false;
  }
  if (errno == ERANGE) {
    cout << "Invalid index range: start too large" << std::endl;
    return false;
  }
  range_text = endptr + 1;
  int64_t index_end = strtol(range_text, &endptr, 10);
  if (index_end < 0 || *endptr != '\0') {
    cout << "Invalid index range: invalid end" << std::endl;
    return false;
  }
  if (errno == ERANGE) {
    cout << "Invalid index range: end too large" << std::endl;
    return false;
  }
  if (index_end < *index_start) {
    cout << "Invalid index range: end before start" << std::endl;
    return false;
  }
  *index_length = index_end - *index_start + 1;
  return true;
}

void MediaPresentationDescriptionParser::ParseSegmentList(
    xmlTextReaderPtr reader,
    const std::string& base_url) const {
  std::unique_ptr<std::string> new_base_url =
      std::unique_ptr<std::string>(new std::string(base_url));
  xmlNodePtr node = xmlTextReaderCurrentNode(reader);
  int64_t timescale;
  int64_t presentation_time_offset;
  int64_t duration;
  int32_t start_number;
  if (!ParseLong(node, "timescale", &timescale, 1)) {
    cout << "Failed to parse segment list 'timescale' attribute" << std::endl;
    return;
  }
  if (!ParseLong(node, "presentationTimeOffset", &presentation_time_offset, 0)) {
    cout << "Failed to parse segment list 'presentationTimeOffset' attribute" << std::endl;
    return;
  }
  if (!ParseLong(node, "duration", &duration, -1)) {
    cout << "Failed to parse segment list 'duration' attribute" << std::endl;
    return;
  }
  if (!ParseInt(node, "startNumber", &start_number, 1)) {
    cout << "Failed to parse segment list 'startNumber' attribute" << std::endl;
    return;
  }

  int ret;
  int parent_depth = xmlTextReaderDepth(reader);
  int depth;
  if (xmlTextReaderIsEmptyElement(reader)) {
    const char* name = (const char*) xmlTextReaderConstName(reader);
    if (name == nullptr) {
      name = "none";
    }
    cout << __func__ << " Emptry Element: " << name << std::endl;
  } else
  do {
    ret = xmlTextReaderRead(reader);
    if (ret != 1) {
      break;
    }
    depth = xmlTextReaderDepth(reader);
    xmlNodePtr child = xmlTextReaderCurrentNode(reader);
    if (NodeNameEquals(child, "Initialization")) {
      ParseInitialization(reader, new_base_url.get());
      NextSibling(reader);
    } else if (NodeNameEquals(child, "SegmentTimeline")) {
      ParseSegmentTimeline(reader);
    } else if (NodeNameEquals(child, "SegmentURL")) {
      ParseSegmentUrl(reader, new_base_url.get());
    }
  } while (depth > parent_depth);
}

void MediaPresentationDescriptionParser::ParseSegmentTemplate(
    xmlTextReaderPtr reader,
    const std::string& base_url) const {
  std::unique_ptr<std::string> new_base_url =
      std::unique_ptr<std::string>(new std::string(base_url));
  xmlNodePtr node = xmlTextReaderCurrentNode(reader);

  int64_t timescale;
  int64_t presentation_time_offset;
  int64_t duration;
  int32_t start_number;
  if (!ParseLong(node, "timescale", &timescale, 1)) {
    cout << "Failed to parse segment template 'timescale' attribute" << std::endl;
    return;
  }
  if (!ParseLong(node, "presentationTimeOffset", &presentation_time_offset, 0)) {
    cout << "Failed to parse segment template 'presentationTimeOffset' attribute" << std::endl;
    return;
  }
  if (!ParseLong(node, "duration", &duration, -1)) {
    return;
  }
  if (!ParseInt(node, "startNumber", &start_number, 1)) {
    cout << "Failed to parse segment template 'startNumber' attribute" << std::endl;
    return;
  }

  ParseUrlTemplate(reader, "media");
  ParseUrlTemplate(reader, "initialization");

  cout << "GIRISH: MediaPresentationDescriptionParser::ParseSegmentTemplate:" <<
               " timescale: " << timescale <<
               " presentationTimeOffset: " << presentation_time_offset <<
               " duration: " << duration <<
               " startNumber: " << start_number << std::endl;
  int ret;
  int parent_depth = xmlTextReaderDepth(reader);
  int depth;
  if (xmlTextReaderIsEmptyElement(reader)) {
    const char* name = (const char*) xmlTextReaderConstName(reader);
    if (name == nullptr) {
      name = "none";
    }
    cout << __func__ << " Emptry Element: " << name << std::endl;
  } else
  do {
    ret = xmlTextReaderRead(reader);
    if (ret != 1) {
      break;
    }
    depth = xmlTextReaderDepth(reader);
    xmlNodePtr child = xmlTextReaderCurrentNode(reader);
    cout << "GIRISH: MediaPresentationDescriptionParser::ParseSegmentTemplate:" <<
                 " name: " << child->name << " depth: " << depth << " line: " << xmlGetLineNo(child) << std::endl;
    if (NodeNameEquals(child, "Initialization")) {
      ParseInitialization(reader, new_base_url.get());
      NextSibling(reader);
    } else if (NodeNameEquals(child, "SegmentTimeline")) {
      ParseSegmentTimeline(reader);
    }
  } while (depth > parent_depth);
}

void MediaPresentationDescriptionParser::ParseSegmentTimeline(
    xmlTextReaderPtr reader) const {
  int64_t elapsed_time = 0;
  int ret;
  int parent_depth = xmlTextReaderDepth(reader);
  int depth;
  if (xmlTextReaderIsEmptyElement(reader)) {
    const char* name = (const char*) xmlTextReaderConstName(reader);
    if (name == nullptr) {
      name = "none";
    }
    cout << __func__ << " Emptry Element: " << name << std::endl;
  } else
  do {
    ret = xmlTextReaderRead(reader);
    if (ret != 1) {
      break;
    }
    depth = xmlTextReaderDepth(reader);
    xmlNodePtr child = xmlTextReaderCurrentNode(reader);
    cout << "GIRISH: MediaPresentationDescriptionParser::ParseSegmentTimeline:" <<
                 " name: " << child->name << " depth: " << depth << " line: " << xmlGetLineNo(child) << std::endl;
    if (NodeNameEquals(child, "S")) {
      if (!ParseLong(child, "t", &elapsed_time, elapsed_time)) {
        cout << "Failed to parse segment timeline 't' attribute" << std::endl;
        return;
      }
      int64_t duration;
      if (!ParseLong(child, "d", &duration)) {
        cout << "Failed to parse segment timeline 'd' attribute" << std::endl;
        return;
      }
      int32_t r_value;
      if (!ParseInt(child, "r", &r_value, 0)) {
        cout << "Failed to parse segment timeline 'r' attribute" << std::endl;
        return;
      }
      int count = 1 + r_value;
      for (int i = 0; i < count; i++) {
        cout << "elapsed_time: " << elapsed_time << " duration: " << duration << std::endl;
        elapsed_time += duration;
      }
      cout << "GIRISH: elapsed_time: " << elapsed_time <<
              " duration: " << duration << " r: " << r_value << std::endl;
      // Call next sibling to avoid duplicate read for </S> nodes
      NextSibling(reader);
    }
  } while (depth > parent_depth);
}

void MediaPresentationDescriptionParser::ParseUrlTemplate(
    xmlTextReaderPtr reader,
    const std::string& name) const {
  xmlNodePtr node = xmlTextReaderCurrentNode(reader);
  std::string valueString = GetAttributeValue(node, name);
  if (!valueString.empty()) {
    cout << "GIRISH: ParseUrlTemplate: name: " << name << " Value: " << valueString << std::endl;
  }
}

void MediaPresentationDescriptionParser::ParseInitialization(
    xmlTextReaderPtr reader,
    const std::string* base_url) const {
  return ParseRangedUrl(reader, base_url, "sourceURL", "range");
}

void MediaPresentationDescriptionParser::ParseSegmentUrl(
    xmlTextReaderPtr reader,
    const std::string* base_url) const {
  return ParseRangedUrl(reader, base_url, "media", "mediaRange");
}

void MediaPresentationDescriptionParser::ParseRangedUrl(
    xmlTextReaderPtr reader,
    const std::string* base_url,
    const std::string& url_attribute,
    const std::string& range_attribute) const {
  xmlNodePtr node = xmlTextReaderCurrentNode(reader);
  std::string urlText = GetAttributeValue(node, url_attribute);
  int64_t range_start = 0;
  int64_t range_length = -1;
  std::string range_text = GetAttributeValue(node, range_attribute);
  if (!range_text.empty() &&
      !ParseRange(range_text, &range_start, &range_length)) {
    cout << "Failed to parse 'range' attribute" << std::endl;
    return;
  }
}

int32_t MediaPresentationDescriptionParser::ParseAudioChannelConfiguration(
    xmlTextReaderPtr reader) const {
  int32_t audio_channels;
  xmlNodePtr node = xmlTextReaderCurrentNode(reader);
  std::string scheme_id_uri = ParseString(node, "schemeIdUri", "");
  if ("urn:mpeg:dash:23003:3:audio_channel_configuration:2011" ==
      scheme_id_uri) {
    if (!ParseInt(node, "value", &audio_channels, -1)) {
      cout << "Audio channel configuration has bad value" << std::endl;
    }
  } else {
    audio_channels = -1;
  }
  return audio_channels;
}

// Utility methods.
std::string MediaPresentationDescriptionParser::CheckLanguageConsistency(
    const std::string& firstLanguage,
    const std::string& secondLanguage) const {
  if (firstLanguage.empty()) {
    return secondLanguage;
  } else if (secondLanguage.empty()) {
    return firstLanguage;
  } else {
    return firstLanguage;
  }
}

AdaptationType MediaPresentationDescriptionParser::CheckContentTypeConsistency(
    AdaptationType firstType,
    AdaptationType secondType) const {
  if (firstType == AdaptationType::UNKNOWN) {
    return secondType;
  } else if (secondType == AdaptationType::UNKNOWN) {
    return firstType;
  } else {
    return firstType;
  }
}

bool MediaPresentationDescriptionParser::ParseFrameRate(xmlNodePtr node,
                                                        double* out,
                                                        double default_value) {
  double frame_rate = default_value;
  std::string frame_rate_attr = GetAttributeValue(node, "frameRate");
  if (!frame_rate_attr.empty()) {
    std::istringstream f(frame_rate_attr);
    std::string numerator;
    std::string denomniator = "1";
    if (std::getline(f, numerator, '/')) {
      std::getline(f, denomniator, '/');
    }

    // Frame rate numerator and denominator are both integers and base 10.
    int32_t n;
    int32_t d;
    if (!ParseInt(numerator, &n)) {
      cout << "Could not parse numerator for frame rate" << std::endl;
      return false;
    }
    if (!ParseInt(denomniator, &d)) {
      cout << "Could not parse denominator for frame rate" << std::endl;
      return false;
    }
    frame_rate = (double)n / (double)d;
  }
  *out = frame_rate;
  return true;
}

int64_t MediaPresentationDescriptionParser::ParseDuration(
    xmlNodePtr node,
    const std::string& name,
    int64_t default_value) {
  xmlChar* value = xmlGetProp(node, TO_XMLCHAR(name.c_str()));
  if (value != nullptr) {
    std::string v(FROM_XMLCHAR(value));
    xmlFree(value);
    return ParseXsDuration(v);
  }
  return default_value;
}

std::string MediaPresentationDescriptionParser::GetAttributeValue(
    xmlNodePtr node,
    const std::string& name,
    std::string default_value) {
  xmlChar* value = xmlGetProp(node, TO_XMLCHAR(name.c_str()));
  if (value != nullptr) {
    std::string v(FROM_XMLCHAR(value));
    xmlFree(value);
    return v;
  }
  return default_value;
}

int64_t MediaPresentationDescriptionParser::ParseDateTime(
    xmlNodePtr node,
    const std::string& name,
    int64_t default_value) {
  xmlChar* value = xmlGetProp(node, TO_XMLCHAR(name.c_str()));
  if (value != nullptr) {
    std::string v(FROM_XMLCHAR(value));
    xmlFree(value);
    return ParseXsDateTime(v);
  }
  return default_value;
}

std::string MediaPresentationDescriptionParser::ParseBaseUrl(
    xmlTextReaderPtr reader,
    const std::string& parent_base_url) {
  std::string base_url = NextText(reader);
  return Resolve(parent_base_url, base_url);
}

bool MediaPresentationDescriptionParser::ParseInt(xmlNodePtr node,
                                                  const std::string& name,
                                                  int32_t* out,
                                                  int32_t default_value) {
  std::string str = GetAttributeValue(node, name);
  *out = default_value;
  if (!str.empty() && !ParseInt(str, out)) {
    return false;
  }
  return true;
}

bool MediaPresentationDescriptionParser::ParseLong(xmlNodePtr node,
                                                   const std::string& name,
                                                   int64_t* out,
                                                   int64_t default_value) {
  std::string str = GetAttributeValue(node, name);
  *out = default_value;
  if (!str.empty() && !ParseLong(str, out)) {
    return false;
  }
  return true;
}

int32_t MediaPresentationDescriptionParser::ParseInt(const std::string& str,
                                                     int32_t* out) {
  int64_t val;
  if (!ParseLong(str, &val)) {
    return false;
  }
  *out = (int32_t)val;
  return true;
}

int64_t MediaPresentationDescriptionParser::ParseLong(const std::string& str,
                                                      int64_t* out) {
  errno = 0;
  *out = strtoll(str.c_str(), nullptr, 10);
  if (errno == ERANGE) {
    cout << "Value out of range: " << str << std::endl;
    return false;
  } else if (errno == EINVAL) {
    cout << "Invalid value: " << str << std::endl;
    return false;
  }
  return true;
}

std::string MediaPresentationDescriptionParser::ParseString(
    xmlNodePtr node,
    const std::string& name,
    const std::string& default_value) {
  xmlChar* value = xmlGetProp(node, TO_XMLCHAR(name.c_str()));
  if (value != nullptr) {
    std::string v(FROM_XMLCHAR(value));
    xmlFree(value);
    return v;
  }
  return default_value;
}

bool MediaPresentationDescriptionParser::CurrentNodeNameEquals(
    xmlTextReaderPtr reader,
    const std::string& name) {
  const xmlChar* tag_name = xmlTextReaderConstName(reader);
  if (tag_name != nullptr) {
    std::string tag_name_str(FROM_XMLCHAR(tag_name));
    return name == tag_name_str;
  }
  return false;
}

bool MediaPresentationDescriptionParser::NodeNameEquals(
    xmlNodePtr child,
    const std::string& name) {
  if (child != nullptr && child->name != nullptr) {
    std::string tag_name_str(FROM_XMLCHAR(child->name));
    return name == tag_name_str;
  }
  return false;
}

std::string MediaPresentationDescriptionParser::NextText(
    xmlTextReaderPtr reader) {
  int ret;
  int parent_depth = xmlTextReaderDepth(reader);
  std::string text_value;
  int depth;
  if (xmlTextReaderIsEmptyElement(reader)) {
    const char* name = (const char*) xmlTextReaderConstName(reader);
    if (name == nullptr) {
      name = "none";
    }
    cout << __func__ << " Emptry Element: " << name << std::endl;
  } else
  do {
    ret = xmlTextReaderRead(reader);
    if (ret != 1) {
      break;
    }
    depth = xmlTextReaderDepth(reader);
    xmlNodePtr child = xmlTextReaderCurrentNode(reader);
    if (depth == parent_depth + 1 && child->type == XML_TEXT_NODE) {
      xmlChar* v = xmlNodeGetContent(child);
      text_value.append(std::string(FROM_XMLCHAR(v)));
      xmlFree(v);
    }
  } while (depth > parent_depth);

  return text_value;
}

void MediaPresentationDescriptionParser::NextSibling(xmlTextReaderPtr reader) {
  int ret;
  int parent_depth = xmlTextReaderDepth(reader);
  int depth;
  if (xmlTextReaderIsEmptyElement(reader)) {
    const char* name = (const char*) xmlTextReaderConstName(reader);
    if (name == nullptr) {
      name = "none";
    }
    cout << __func__ << " Emptry Element: " << name << std::endl;
  } else
  do {
    ret = xmlTextReaderRead(reader);
    if (ret != 1) {
      break;
    }
    depth = xmlTextReaderDepth(reader);
  } while (depth > parent_depth);
}
