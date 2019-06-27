#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include <gtest/gtest.h>


struct InitHook {
    ~InitHook()
    {
        std::cout << "Enter RETURN to exit" << std::endl;
        std::cin.get();
    }
};

#ifdef _WIN32
// InitHook inithook;
#endif
// ==================== Implementation ========================

int factorial(int n){
    int prod = 1;
    for(int i = 1; i <= n; i++)
        prod *= i;
    return prod;
}

struct Date
{
    int year;
    int month;
    int day;
    Date(){}
    Date(int year, int month, int day)
        : year(year)
        , month(month)
        , day(day)
    { }

    // Comparison operator required by  EXPECT_EQ
    bool operator==(Date const& rhs) const
    {
        return year  == rhs.year
               && month == rhs.month
               && day   == rhs.day;
    }
    // Necessary for make class printable in GTest
    friend std::ostream& operator<<(std::ostream& os, Date const& rhs)
    {
        return os << "Date { " << rhs.year << " ; "
                  << rhs.month << " ; "
                  << rhs.day << " } ";
    }
};

Date GregorianEasterSunday(int y)
{
    int c = y / 100;
    int n = y - 19 * ( y / 19 );
    int k = ( c - 17 ) / 25;
    int i = c - c / 4 - ( c - k ) / 3 + 19 * n + 15;
    i = i - 30 * ( i / 30 );
    i = i - ( i / 28 ) * ( 1 - ( i / 28 )
                                * ( 29 / ( i + 1 ) )
                                * ( ( 21 - n ) / 11 ) );
    int j = y + y / 4 + i + 2 - c + c / 4;
    j = j - 7 * ( j / 7 );
    int l = i - j;
    int m = 3 + ( l + 40 ) / 44;
    int d = l + 28 - 31 * ( m / 4 );
    return Date(y, m, d);
}

//=============== Tests ====================================//

TEST(FactorialTest, test1){
    EXPECT_EQ(6,        factorial(3));
    EXPECT_EQ(24,       factorial(4));
    EXPECT_EQ(120,      factorial(5));
    EXPECT_EQ(3628800,  factorial(10));
    // Expect greater than
    EXPECT_GT(10000000, factorial(10));
    // Expect not equal
    EXPECT_NE(25, factorial(4));
}

TEST(FactorialTestFailure, testFailure){
    // Deliberately fails for demonstration purposes
    EXPECT_EQ(6, factorial(3));
    EXPECT_EQ(4, factorial(4));
    EXPECT_EQ(6, factorial(2));
}

TEST(GregorianEaster, testdates){
    EXPECT_EQ(Date(2005, 3, 27), GregorianEasterSunday(2005));
    EXPECT_EQ(Date(2008, 3, 23), GregorianEasterSunday(2008));
    EXPECT_EQ(Date(2010, 4, 4),  GregorianEasterSunday(2010));

    // Failure
    EXPECT_EQ(Date(2010, 14, 4), GregorianEasterSunday(2010));
}
