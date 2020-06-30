/*
decoder(encoder(input)) == input
size(encoder(input)) < size(input)  ... when possible

"12xa" => "aaaaaaaaaaaa"
"12x" => "12x"
"12a" => "12a"
"a4xbc" => "abbbbc"
"x12ab" => "x12ab"
 
 
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
    } else {
      output.append(item);
    }
  }
}

