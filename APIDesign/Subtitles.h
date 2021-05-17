/*
 * Design an API that delivers Subtitles/Closed Caption in timely manner.
 * Design considerations:
 *     - Should support different types of CC/Subtitles - such as CEA608, CEA708, WebVTT
 *     - Should consider both polling (synchronous) and callbacks (asynchronous)
 *     - Graceful error handling
 *     - Should be expandable to future needs 
 */


/*
  1. Correlate the closed caption with time stamp - start time, end time
  2. Abstraction for closed caption text - takes care of how its presented/rendered
  3. Encoding of the Character set - UTF16
  4. Formatting associated with the text
 */

//Polling - sync
bool GetSubtitles(const time_t& time, Abstraction& subtitles);

//Async- callack
void RegisterForSubtitles();
void Callback(const time_t& time, Abstraction& subtitles);

//tuple[0] => 
//   => if ok the != nullptr 
//   => if no subtitles == nullptr -> there are no subtitles
//tuple[1] 
//    => if ok then == nullptr
//    => if nok then != nullptr
std::tuple<std::unique_ptr<Abstraction>, std::unique_ptr<Error>> GetSubtitles(const time_t& time);

//define FAILED_TO_REGISTER INT_MIN
//if the return value is greater then 0 then success
int RegisterForSubtitles(
  std::function<void, std::unique_ptr<Abstraction>> callback, 
  std::function<void, std::unique_ptr<Error>> error_callback);

bool UnregisterForSubtitles(int subscription_id);


