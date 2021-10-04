rm -f a.out
SOURCE="Main.cpp Node.cpp NodeCollection.cpp PhoneBook.cpp"
cc $SOURCE -lstdc++ -std=c++14 -Wall -o a.out
