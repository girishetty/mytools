clear
rm -f mpd_parser
gcc -g -std=c++11 main.cc mpd_parser.cc normalizer.cc -I/usr/include/libxml2 -lstdc++ -lxml2 -o mpd_parser
