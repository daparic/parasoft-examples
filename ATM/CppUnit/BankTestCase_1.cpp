#include "BankTestCase_1.h"
#include "Bank.hxx"
#include "Account.hxx"

CPPUNIT_TEST_SUITE_REGISTRATION(BankTestCase);

BankTestCase::BankTestCase()
{
}


BankTestCase::~BankTestCase()
{
}


void 
BankTestCase::setUp()
{
}


void 
BankTestCase::tearDown()
{
}


void 
BankTestCase::testGetAccount()
{
    Bank* bankWithTwoAccounts = new Bank;
    Account* account1 = bankWithTwoAccounts->addAccount();
    account1->setPassword("password1");
    Account* account2 = bankWithTwoAccounts->addAccount();
    account2->setPassword("password2");

    int _num  = 0;

    /* Initializing argument 2 (password) */ 
    string _password  = "password1";

    /* Tested function call */
    Account * _return  = bankWithTwoAccounts->getAccount(_num, _password);

    CPPUNIT_ASSERT(_return != 0)
}

