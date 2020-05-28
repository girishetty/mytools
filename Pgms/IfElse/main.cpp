# include <string.h>

#define OK              0
#define ERR_MEMORY      1
#define ERR_ARGUMENT    2

typedef struct {
    char* name;
    //...
} ContactDetails;

#if 1
int AddContacts(ContactDetails* pContacts)
{
    int res = OK;

    if (pContacts) {
        ContactDetails* contacts = new ContactDetails;
        if (contacts) {
            if (pContacts->name) {
                contacts->name = new char[strlen(pContacts->name)];
                if (contacts->name) {
                    strcpy(contacts->name, pContacts->name);
                    //copy other data similarly...
                    //...
                } else {
                    res = ERR_MEMORY;
                }
            } else {
                res = ERR_ARGUMENT;
            }
        } else {
            res = ERR_MEMORY;
        }
    } else {
        res = ERR_ARGUMENT;
    }

    return res;
}

# else

int AddContacts(ContactDetails* pContacts)
{
    int res = ERR_ARGUMENT;

    if (pContacts) {
        //set error as ERR_MEMORY
        res = ERR_MEMORY;
        ContactDetails* contacts = new ContactDetails;
        if (contacts) {
            //set error as ERR_ARGUMENT
            res = ERR_ARGUMENT;
            if (pContacts->name) {
                //set error as ERR_MEMORY
                res = ERR_MEMORY;
                contacts->name = new char[strlen(pContacts->name)];
                if (contacts->name) {
                    strcpy(contacts->name, pContacts->name);
                    //copy other data similarly...
                    //...
                    res = OK;
                }
            }
        }
    }

    return res;
}
#endif

int main()
{
    ContactDetails* pContacts = NULL;
    AddContacts(pContacts);

    return 0;
}
