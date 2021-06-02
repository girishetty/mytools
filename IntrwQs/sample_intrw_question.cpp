
/*
You are given with an API that makes an HTTP request for a given URL and possible HTTP
Request headers

Callback as and when Response Header is available:
typedef size_t (*ProcessHttpsHeader)(const std::string* header);

Callback as and when Response body is available:
typedef size_t (*ProcessHttpsBody*)(const std::vector<int8_t>* pBody);

int make_http_connection(const std::string& url,
                         const std::vector<string>& http_request_header,
                         ProcessHttpsHeader process_header,
                         ProcessHttpsBody process_body);

HTTP Response header can be something like this (an example) with every ProcessHttpsHeader
callback giving one line of response header ending with \r\n
When all the headers are done, tht is notified by issing ProcessHttpsHeader with just \r\n

Example:
HTTP/x.y <ResponseCode>
Transfer-Encoding: chunked
Date: Sat, 19 Nov 2018 08:36:25 GMT
Server: <ServerName>
Connection: close
Expires: Sat, 28 Nov 2009 05:36:25 GMT
Etag: "pub1259380237;gz"
Cache-Control: max-age=3600, public
Content-Type: text/html; charset=UTF-8
Last-Modified: Sat, 18 Nov 2018 03:50:37 GMT
Content-Encoding: gzip
Set-Cookie: name1=value1; <name2=value2; ... nameN=valueN>

There can be multiple Set-Cookie Headers

Some of the <ResponseCode> can be:
200 OK, 
206 Partial Content
301 Moved Permanently
302 Moved Temporarily
307 Temporary Redirect
403 Forbidden

The task is to process Set-Cookie response header and maintain them so that, if the <ResponseCode>
is "403 Forbidden", then we should be able to resend the HTTP request with the request header "Cookie:"
that contains all the <name=value> pairs that we have got from Response headers, seperated by ;

NOTE that:
  1. There can be maximum of 50 cookie per HTTP Request
  2. The Size of the "Cookie: xyz" request header can't be more than 4093 bytes
*/
