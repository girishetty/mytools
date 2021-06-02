#pragma once

#include <string>

struct _xmlNode;
typedef struct _xmlNode xmlNode;
typedef xmlNode *xmlNodePtr;
struct _xmlTextReader;
typedef struct _xmlTextReader xmlTextReader;
typedef xmlTextReader *xmlTextReaderPtr;

enum class AdaptationType {
  UNKNOWN = -1,
  VIDEO = 0,
  AUDIO = 1,
  TEXT = 2,
};

class MediaPresentationDescriptionParser {
 public:
  MediaPresentationDescriptionParser(std::string content_id = "");

  void Parse(const std::string& xml, const std::string& connection_url) const;

 private:
  std::string content_id_;

  void ParseMediaPresentationDescription(xmlTextReaderPtr reader, const std::string& base_url) const;

  void ParseDescriptorType(xmlNodePtr node) const;

  void ParsePeriod(xmlTextReaderPtr reader, const std::string& base_url, int64_t default_start_ms) const;

  void ParseAdaptationSet(xmlTextReaderPtr reader, const std::string& base_url) const;

  AdaptationType ParseContentType(xmlNodePtr node) const;

  static AdaptationType GetContentType(const std::string& mime_type);

  void ParseContentProtection(xmlTextReaderPtr reader) const;

  void ParseRepresentation(
      xmlTextReaderPtr reader,
      const std::string& base_url,
      const std::string& adaptation_set_mime_type,
      const std::string& adaptation_set_codecs,
      int32_t adaptation_set_width,
      int32_t adaptation_set_height,
      float adaptation_set_frame_rate,
      int32_t max_playout_rate,
      int32_t adaptation_set_audio_channels,
      int32_t adaptation_set_audio_samplingRate,
      const std::string& adaptation_set_language) const;

  void ParseSegmentBase(xmlTextReaderPtr reader, const std::string& base_url) const;

  bool ParseRange(const std::string& str, int64_t* start, int64_t* length) const;

  void ParseSegmentList(xmlTextReaderPtr reader, const std::string& base_url) const;

  void ParseSegmentTemplate(xmlTextReaderPtr reader, const std::string& base_url) const;

  void ParseSegmentTimeline(xmlTextReaderPtr reader) const;

  void ParseUrlTemplate(xmlTextReaderPtr reader, const std::string& name) const;

  void ParseInitialization(xmlTextReaderPtr reader, const std::string* base_url) const;

  void ParseSegmentUrl(xmlTextReaderPtr reader, const std::string* base_url) const;

  void ParseRangedUrl(xmlTextReaderPtr reader, const std::string* base_url,
      const std::string& url_attribute, const std::string& range_attribute) const;

  int32_t ParseAudioChannelConfiguration(xmlTextReaderPtr reader) const;

  static bool ParseFrameRate(xmlNodePtr node, double* out, double default_value);

  static std::string GetAttributeValue(xmlNodePtr node, const std::string& name, std::string default_value = "");

  static int64_t ParseDuration(xmlNodePtr node, const std::string& name, int64_t default_value);

  static int64_t ParseDateTime(xmlNodePtr node, const std::string& name, int64_t default_value);

  static std::string ParseBaseUrl(xmlTextReaderPtr reader, const std::string& parent_base_url);

  static bool ParseInt(xmlNodePtr node, const std::string& name, int32_t* out, int32_t default_value = -1);

  static bool ParseLong(xmlNodePtr node, const std::string& name, int64_t* out, int64_t default_value = -1);

  static int32_t ParseInt(const std::string& str, int32_t* out);

  static int64_t ParseLong(const std::string& str, int64_t* out);

  static std::string ParseString(xmlNodePtr node,
                                 const std::string& name,
                                 const std::string& default_value);

  static bool CurrentNodeNameEquals(xmlTextReaderPtr reader,
                                    const std::string& name);

  static bool NodeNameEquals(xmlNodePtr node, const std::string& name);

  static std::string NextText(xmlTextReaderPtr reader);

  static void NextSibling(xmlTextReaderPtr reader);

  std::string CheckLanguageConsistency(
      const std::string& first_language,
      const std::string& second_language) const;
  AdaptationType CheckContentTypeConsistency(
       AdaptationType first_type,
       AdaptationType second_type) const;

};
