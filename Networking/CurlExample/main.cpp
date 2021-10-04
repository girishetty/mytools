#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>

static size_t header_callback(char* buffer, size_t size, size_t nitems, void* userdata) {
  (void)userdata;
  printf("Header: [%s]", buffer);
  return size * nitems;
}

size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
  (void)userdata;
  (void)ptr;
  printf("Body: [%zu]\n", size * nmemb);
  return size * nmemb;
}

int main() {
  curl_global_init(CURL_GLOBAL_ALL);
  CURLM* multi_handle = curl_multi_init();
 
  CURL* easy_handle = curl_easy_init();
  curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, header_callback);
  curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(easy_handle, CURLOPT_URL, "www.google.com");
  curl_multi_add_handle(multi_handle, easy_handle);
 
  int still_alive = 1;
  do {
    int numfds = 0;
    CURLMcode mc = curl_multi_perform(multi_handle, &still_alive);
    if (mc == CURLM_OK ) {
      /* wait for activity, timeout or "nothing" */
      mc = curl_multi_wait(multi_handle, NULL, 0, 1000, &numfds);
    }
   
    if (mc != CURLM_OK) {
      printf("curl_multi failed, code %d\n", mc);
      still_alive = 0;
    } else if (numfds == 0) {
      // sleep briefly
      usleep(100000);
    }
  } while (still_alive);
 
  curl_multi_cleanup(multi_handle);
  curl_global_cleanup();
 
  return 0;
}

