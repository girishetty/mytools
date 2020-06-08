#ifndef _PHONE_BOOK_H
#define _PHONE_BOOK_H

class Node;
class Entry;

class PhoneBook {
public:
  PhoneBook();
  ~PhoneBook();
  void CleanupPhoneBook(Node* pNode);

  void AddEntry(const char* pName, const char* pPhoneNum);
  void AddEntry(const Entry& anEntry);
  bool Find(const char* key) const;
  void ShowEntriesStartingFromKey(const char* key) const;

private:
  Node* FindNode(const char* key) const;

private:
  Node* iRoot = nullptr;
};

#endif //_PHONE_BOOK_H
