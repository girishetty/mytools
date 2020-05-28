#include "LogicGates.h"

int main() {

	NotLogic* pNot = new NotLogic;
	char notInput = 't';
	char notOutput = pNot->GetOutput(&notInput, 1);
	cout<<"NOT of "<<notInput<<" is "<<notOutput<<endl;

	OrLogic* pOr = new OrLogic;
	char* orInput = "tfffft";
	char orOutput = pOr->GetOutput(orInput, sizeof(orInput));
	cout<<"OR of "<<orInput<<" is "<<orOutput<<endl;

	
	AndLogic* pAnd = new AndLogic;
	char* andInput = "tfffft";
	char andOutput = pAnd->GetOutput(andInput, sizeof(andInput));
	cout<<"AND of "<<andInput<<" is "<<andOutput<<endl;
	
	pAnd->GetTruthTable();

	delete pNot;
	delete pOr;
	delete pAnd;

	return 0;
}
