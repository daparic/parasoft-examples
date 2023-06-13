#ifndef BANKTESTCASE_H
#define BANKTESTCASE_H

#include <cppunit/extensions/HelperMacros.h>

class BankTestCase : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE( BankTestCase );
  CPPUNIT_TEST( testGetAccount );
  CPPUNIT_TEST_SUITE_END();

public:
  BankTestCase();
  virtual ~BankTestCase();

  virtual void setUp();
  virtual void tearDown();

  void testGetAccount();

private:
  BankTestCase( const BankTestCase &copy );
  void operator =( const BankTestCase &copy );
};


#endif  // BANKTESTCASE_H
