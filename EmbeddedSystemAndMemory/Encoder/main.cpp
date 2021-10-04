/*

We have a stream of characters that needs to be processed.
Before processing them, they are encoded and transmitted.
At the receiving end, they are decoded and processed.

Here, what we want to do is implement an Encoder so that
whenever possible, the size of encoded input is smaller than
size of the actual input.

size(encoder(input)) < size(input) ==>  { Whenever possible }
decoder(encoder(input)) == input

So, the Encoder algorithm works this way, for any sequence/stream of
characters, if its repeating multiple times, add the count first followed
by x and the actual character (that is repeating)
i.e CCCCCCC is encoded as 7xC

Example:
encoder("aaaaaaaaaaaa") ==> "12xa"
encoder("12x") ==> "12x"
encoder("12a") ==> "12a"
encoder("abbbbc") => "a4xbc"
encoder("x12ab") => "x12ab"

decoder("12xa") ==> "aaaaaaaaaaaa"
decoder("12x") ==> "12x"
decoder("12a") ==> "12a"
decoder("a4xbc") => "abbbbc"
decoder("x12ab") => "x12ab"

encode(4xA) ==> 1x41xxA
*/
  

void encoder(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
  output.clear();
  for (size_t position = 0; position < input.size();) {
    auto item = input[position];
    size_t count = 1;
    for (; position < input.size() && input[++position] == item; count++);
    if (count > 3) {
      auto times = std::itoa(count);
      output.append(times);
      output.append('x');
      output.append(item);
    } else if (count == 1) {
    } else {
      output.append(item, count);
    }
  }
}

