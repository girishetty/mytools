#include <iostream>
using namespace std;
#include "PhoneBook.h"

int main() {
	PhoneBook* pb = new PhoneBook();
	pb->AddEntry("Girish", "12345");
	pb->AddEntry("Ganesh", "44445");
	pb->AddEntry("Gita", "71345");
	pb->AddEntry("Gautam", "55345");
	pb->AddEntry("Ashwin", "22345");
	pb->AddEntry("Ashish", "12335");
	pb->AddEntry("Sudhir", "84345");
	pb->AddEntry("Asha", "83345");
	pb->AddEntry("Deepthi", "95345");
	pb->AddEntry("Amma", "93345");
	pb->AddEntry("Praveen", "64345");
	pb->AddEntry("Subhra", "71345");
	pb->AddEntry("Sirisha", "14945");
	pb->AddEntry("Prathish", "87345");
	pb->AddEntry("Dushyant", "35345");
	pb->AddEntry("Sanket", "56345");
	pb->AddEntry("Praveen", "12995");
	pb->AddEntry("Supreetha", "88345");
	pb->AddEntry("Harish", "10945");
	pb->AddEntry("Deepak", "89345");
	pb->AddEntry("Suprabha", "56345");
	pb->AddEntry("Amanda", "12785");
	pb->AddEntry("Harsha", "12455");

	pb->ShowEntriesStartingFromKey("Gi");
	pb->ShowEntriesStartingFromKey("D");
	pb->ShowEntriesStartingFromKey("Su");
	pb->ShowEntriesStartingFromKey("A");

    if(pb->Find("Girish") == true) {
        cout<<"Entry Found"<<endl;
    }
    else {
        cout<<"Entry Not Found"<<endl;
    }

	cout<<"Do clean up of PhoneBook"<<endl;
	delete pb;

	return 0;
}
