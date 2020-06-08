class MandatoryRole {
};

class RoleA {
};

class RoleB {
};

class Component : public MandatoryRole,
                  private RoleA,
                  private RoleB
{

public:
  //define and expose conversion functions to cause private inheritance valid
  operator RoleA*() {
    RoleA* pA = nullptr;
    //Depending on the precondition of RoleA, return "this"
    if(pre_conditions)
      pA = (RoleA*) this;
    return pA;
  }

  operator RoleB*() {
    RoleA* pB = nullptr;
    //Depending on the precondition of RoleB, return "this"
    if(pre_conditions)
      pA = (RoleB*) this;
    return pB;  
  }

  //Or expose below functions to cause private inheritance valid
  RoleA* ConfigureForRoleA() {
    //Internally this calls operator RoleA*()
    return *this;
  }

  RoleB* ConfigureForRoleB() {
    //Internally this calls operator RoleB*()
    return *this;
  }
};

//Usage code
void Function(Component& comp) {
  RoleA* pA = comp;  //if comp is pointer to Component, then this code should be changed as "RoleA* pA = *comp;"
  RoleB* pB = comp;
}


//Usage code
void Function(Component* comp) {
  RoleA* pA = comp->ConfigureForRoleA();  
  RoleB* pB = comp->ConfigureForRoleB();
}

class Derived : private Base { };

Base* p = new Derived; //Is an error
