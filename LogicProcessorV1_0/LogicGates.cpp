#include "LogicGates.h"

const int KSingleInput = 1;

/***********************************************************************
 Abstract Base class Function which sets input from given source of input
 ***********************************************************************/
void Logic::SetInputsFrom(bool* aToInput, const bool* const aFromInput, int aNumInputs){
    for (int i=0;i<aNumInputs;i++) {
        aToInput[i] = aFromInput[i];
    }
}

/***********************************************************************
 Definition of BoolLogic Class
 ***********************************************************************/
//Constructer and destructer
BoolLogic::BoolLogic(bool aInput) : iInput(aInput){
}

BoolLogic::BoolLogic(Logic& aInput) : iInput(aInput.Process()) {
}

BoolLogic::BoolLogic(Logic* const aInput) : iInput(aInput->Process()) {
}

BoolLogic::~BoolLogic() {
}

//Implementation of Bool Logic
const bool BoolLogic::Process() {
    return iInput;
}

const bool BoolLogic::Process(const bool* const aInput) {
    return *aInput;
}

void BoolLogic::SetInputs(const bool* const aInput) {
        iInput = *aInput;
}

    
/***********************************************************************
 Definition of NOT Class
 ***********************************************************************/
//Constructer and destructer
NotLogic::NotLogic(){
}

NotLogic::NotLogic(const bool* const aInput){
    SetInputsFrom(&iInput, aInput, KSingleInput);
}

NotLogic::NotLogic(Logic& aInput){
    iInput = aInput.Process();
}

NotLogic::NotLogic(Logic** const aInput){
    iInput = aInput[0]->Process();
}


NotLogic::~NotLogic() {
}

//Implementation of NOT Logic
const bool NotLogic::Process(){
    return !iInput;
}

const bool NotLogic::Process(const bool* const aInput){
    return !(*aInput);
}

void NotLogic::SetInputs(const bool* const aInput){
    SetInputsFrom(&iInput, aInput, KSingleInput);
}

/***********************************************************************
 Definition of AND Class
 ***********************************************************************/
//Constructer and destructer
AndLogic::AndLogic(int aNumInputs ) :
    iNumInputs(aNumInputs<KMinInput ? KMinInput:aNumInputs) {
    iInputs = new bool[iNumInputs];
}

AndLogic::AndLogic(const bool* const aInput, int aNumInputs) :
    iNumInputs(aNumInputs<KMinInput ? KMinInput:aNumInputs) {
    iInputs = new bool[iNumInputs];
    SetInputsFrom(iInputs, aInput, iNumInputs);
}

AndLogic::AndLogic(Logic& aInput1, Logic& aInput2) : iNumInputs(KMinInput){
    iInputs = new bool[iNumInputs];
    iInputs[0] = aInput1.Process();
    iInputs[1] = aInput2.Process();
}

AndLogic::AndLogic(Logic** const aInput, int aNumInputs) :
    iNumInputs(aNumInputs<KMinInput ? KMinInput:aNumInputs) 
 {
    iInputs = new bool[iNumInputs];
    for (int i=0; i<iNumInputs;i++) {
        iInputs[i] = aInput[i]->Process();
    }
}

AndLogic::~AndLogic() {
    delete []iInputs;
}

//Implementation of AND Logic
const bool AndLogic::Process(){

    return Process(iInputs);
}

const bool AndLogic::Process(const bool* const aInput){
    bool output;
    output = aInput[0];

    //Continue, only if its true
    for (int i=1; i<iNumInputs && output; i++) {
        output &= aInput[i];
    }

    return output;
}

void AndLogic::SetInputs(const bool* const aInput){
    SetInputsFrom(iInputs, aInput, iNumInputs);
}


/***********************************************************************
 Definition of OR Class
 ***********************************************************************/
//Constructer and destructer
OrLogic::OrLogic(int aNumInputs) :
    iNumInputs(aNumInputs<KMinInput ? KMinInput:aNumInputs) {
    iInputs = new bool[iNumInputs];
}

OrLogic::OrLogic(const bool* const aInput, int aNumInputs) :
    iNumInputs(aNumInputs<KMinInput ? KMinInput:aNumInputs) {
    iInputs = new bool[iNumInputs];
    SetInputsFrom(iInputs, aInput, iNumInputs);
}

OrLogic::OrLogic(Logic& aInput1, Logic& aInput2) : iNumInputs(KMinInput){
    iInputs = new bool[iNumInputs];
    iInputs[0] = aInput1.Process();
    iInputs[1] = aInput2.Process();
}

OrLogic::OrLogic(Logic** const aInput, int aNumInputs) :
    iNumInputs(aNumInputs<KMinInput ? KMinInput:aNumInputs) 
 {
    iInputs = new bool[iNumInputs];
    for (int i=0; i<iNumInputs;i++) {
        iInputs[i] = aInput[i]->Process();
    }
}

OrLogic::~OrLogic() {
    delete []iInputs;
}

//Implementation of OR Logic
const bool OrLogic::Process(){

    return Process(iInputs);
}

const bool OrLogic::Process(const bool* const aInput){
    bool output;
    output = aInput[0];

    //Continue, only if its false
    for (int i=1; i<iNumInputs && !output; i++) {
        output |= aInput[i];
    }

    return output;
}

void OrLogic::SetInputs(const bool* const aInput){
    SetInputsFrom(iInputs, aInput, iNumInputs);
}


/***********************************************************************
 Definition of XOR Class
 ***********************************************************************/
//Constructer and destructer
XorLogic::XorLogic(int aNumInputs) :
    iNumInputs(aNumInputs<KMinInput ? KMinInput:aNumInputs) {
    iInputs = new bool[iNumInputs];
}

XorLogic::XorLogic(const bool* const aInput, int aNumInputs) :
    iNumInputs(aNumInputs<KMinInput ? KMinInput:aNumInputs) {
    iInputs = new bool[iNumInputs];
    SetInputsFrom(iInputs, aInput, iNumInputs);
}

XorLogic::XorLogic(Logic& aInput1, Logic& aInput2) : iNumInputs(KMinInput){
    iInputs = new bool[iNumInputs];
    iInputs[0] = aInput1.Process();
    iInputs[1] = aInput2.Process();
}

XorLogic::XorLogic(Logic** const aInput, int aNumInputs) :
    iNumInputs(aNumInputs<KMinInput ? KMinInput:aNumInputs) 
 {
    iInputs = new bool[iNumInputs];
    for (int i=0; i<iNumInputs;i++) {
        iInputs[i] = aInput[i]->Process();
    }
}

XorLogic::~XorLogic() {
    delete []iInputs;
}

//Implementation of XOR Logic
const bool XorLogic::Process(){
    return Process(iInputs);
}

const bool XorLogic::Process(const bool* const aInput){
    bool output;
    output = aInput[0];

    for (int i=1; i<iNumInputs; i++) {
        output ^= aInput[i];
    }

    return output;
}

void XorLogic::SetInputs(const bool* const aInput){
    SetInputsFrom(iInputs, aInput, iNumInputs);
}

/***********************************************************************
 Definition of Implies Class
 ***********************************************************************/
//Constructer and destructer
ImpliesLogic::ImpliesLogic(){
    iInputs = new bool[KMinInput];
}

ImpliesLogic::ImpliesLogic(const bool* const aInput){
    iInputs = new bool[KMinInput];
    SetInputsFrom(iInputs, aInput, KMinInput);
}

ImpliesLogic::ImpliesLogic(Logic& aInput1, Logic& aInput2){
    iInputs = new bool[KMinInput];
    iInputs[0] = aInput1.Process();
    iInputs[1] = aInput2.Process();
}

ImpliesLogic::ImpliesLogic(Logic** const aInput){
    iInputs = new bool[KMinInput];
    iInputs[0] = aInput[0]->Process();
    iInputs[1] = aInput[1]->Process();
}

ImpliesLogic::~ImpliesLogic() {
    delete []iInputs;
}

//Implementation of XOR Logic
const bool ImpliesLogic::Process(){
    return Process(iInputs);
}

const bool ImpliesLogic::Process(const bool* const aInput){
    //If true->false then flase, else its true always
    if (aInput[0] & !aInput[1]){
        return false;
    }
    else { 
        return true;
    }
}

void ImpliesLogic::SetInputs(const bool* const aInput){
    SetInputsFrom(iInputs, aInput, KMinInput);
}
