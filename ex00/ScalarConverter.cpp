#include "ScalarConverter.hpp"
#include <iostream>
#include <iomanip> 
#include <limits>
#include <cmath>
#include <cstdlib>

static const std::string DOUBLE_MAX_STR =
    "179769313486231570814527423731704356798"
    "07056752584499659891747680315726078002"
    "85387605895586327668781715404589535143"
    "82464234321326889464182768467546703537"
    "51698604991057655128207624549009038932"
    "89440758685084551339423045832369032229"
    "48165808559332123348274797826204144723"
    "168738177180919299881250404026184124858368.0";
static const std::string FLOAT_MAX_STR =
    "340282346638528859811704183484516925440.0";

enum LiteralType
{
    CHAR,
    INT,
    FLOAT,
    DOUBLE,
    PSEUDO,
    INVALID
};

static std::string removeTrailingZeros(std::string right)
{
    std::string::size_type p = right.find_last_not_of('0'); 
    if (p == std::string::npos) 
        right.clear();
    else 
        right.erase(p+1);

    return right;
}

static std::string removeOptional(std::string literal)
{
    if((!literal.empty()) && ((literal[0] == '+') || (literal[0] == '-')))
    {
        literal = literal.substr(1);
    }
    return literal;
}

static int checkDots(std::string sub)
{
    size_t indexDot = sub.find('.');
    //check for at least one dot
    if(indexDot == std::string::npos)
    {
        return 0;
    }
    //check for multiple dots
    std::string temp = sub.substr(indexDot+1);  
    if(temp.find('.') != std::string::npos)
    {
        return 0;
    }
    return 1;
}

static int checkDotPos(std::string sub, size_t len)
{
    if ((sub[len-1] == '.') || (sub[0] == '.'))
    {
        return 0;
    }
    return 1;
}

static int isPseudoLiteral(const std::string& literal)
{
    std::string sub = removeOptional(literal);

    if(sub == "nan" || sub == "inf" ||sub == "nanf" || sub == "inff" )
        return 1;
    return 0;
}

static int isCharLiteral(const std::string& literal) // non-displayable characters shouldn’t be used as inputs
{    
    if((literal.size() == 1) && !isdigit(literal[0]))
        return 1;
    return 0;
}

static int isIntLiteral(const std::string& literal)
{
    std::string sub = removeOptional(literal);
    size_t len = sub.size();
    if(sub.empty())
        return 0;
    for (size_t i = 0; i < len; i++)
    {
        if (!isdigit(sub[i]))
            return 0;
    }
    double d = strtod(literal.c_str(), NULL);
    if(d > std::numeric_limits<int>::max() || d < std::numeric_limits<int>::min())
    {
        return 0;
    }
    return 1;
}

static int isFloatLiteral(const std::string& literal)
{
    std::string sub = removeOptional(literal);
    size_t len = sub.size();
    if(sub.empty() || len < 4)
        return 0;
    int last = len - 1;
    if((sub[last] != 'f'))
        return 0;
    sub = sub.substr(0, len-1);
    len = len-1;
    if (!checkDots(sub) || !checkDotPos(sub, len))
    {
        return 0;
    }
    for (size_t i = 0; i < len-1; i++)
    {
        if (!isdigit(sub[i]) && sub[i] !='.')
        {
            return 0;
        }            
    }
    
    std::string::size_type p = sub.find('.');
    std::string left  = sub.substr(0, p);
    std::string right = sub.substr(p+1);

    std::string::size_type pMax = FLOAT_MAX_STR.find('.');
    std::string leftMax  = FLOAT_MAX_STR.substr(0, pMax);
    std::string rightMax = FLOAT_MAX_STR.substr(pMax+1);

    right = removeTrailingZeros(right);
    rightMax = removeTrailingZeros(rightMax);
    
    if (left.size() > leftMax.size())
    {
        return 0;
    }
    else if ((left.size() == leftMax.size()) && (left > leftMax))
    {
        return 0;
    }
    if((left.size() == leftMax.size()) && (left == leftMax) && (right > rightMax))
    {
        return 0;
    }
    return 1;
}

static int isDoubleLiteral(const std::string& literal)
{
    std::string sub = removeOptional(literal);
    size_t len = sub.size();
    if (!checkDots(sub) || !checkDotPos(sub, len))
    {
        return 0;
    }
    for (size_t i = 0; i < len; i++)
    {
        if (!isdigit(sub[i]) && sub[i] !='.')
            return 0;
    }
    std::string::size_type p = sub.find('.');
    std::string left  = sub.substr(0, p);
    std::string right = sub.substr(p+1);

    std::string::size_type pMax = DOUBLE_MAX_STR.find('.');
    std::string leftMax  = DOUBLE_MAX_STR.substr(0, pMax);
    std::string rightMax = DOUBLE_MAX_STR.substr(pMax+1);

    right = removeTrailingZeros(right);
    rightMax = removeTrailingZeros(rightMax);

    if (left.size() > leftMax.size())
    {
        return 0;
    }
    else if ((left.size() == leftMax.size()) && (left > leftMax))
    {
        return 0;
    }
    if((left.size() == leftMax.size())  && (left == leftMax) && (right > rightMax))
    {
        return 0;
    }
    return 1;
}

static LiteralType detectType(const std::string& literal)
{
    if (isPseudoLiteral(literal))
        return PSEUDO;
    if (isCharLiteral(literal))
        return CHAR;
    if (isIntLiteral(literal))
        return INT;
    if (isFloatLiteral(literal))
        return FLOAT;
    if (isDoubleLiteral(literal))
        return DOUBLE;
    return INVALID;
}

static void printValues(
    bool charPossible,
    bool charDisplayable,
    char c,
    bool intPossible,
    int i,
    bool floatPossible,
    float f,
    double d,
    bool pseudo,
    bool hasPlusSign)
{
    if(pseudo == true)
    {
        std::cout << "char: " << "impossible" << std::endl;
        std::cout << "int: " << "impossible" << std::endl;
        if (hasPlusSign)
        {
            std::cout << "float: +" << f << "f" << std::endl;
            std::cout << "double: +" << d << std::endl;
        }
        else
        {
            std::cout << "float: " << f << "f" << std::endl;
            std::cout << "double: " << d << std::endl;
        }
    }
    else
    {        
        //char handling
        if (!charPossible)
        {
            std::cout << "char: " << "impossible" << std::endl;
        }
        else if (!charDisplayable)
        {
            std::cout << "char: " << "Non displayable" << std::endl;
        }       
        else
        {
            std::cout << "char: '" << c << "'" << std::endl;
        }

        //int handling
        if(!intPossible)
        {
            std::cout << "int: " << "impossible" << std::endl;
        }
        else
        {      
            std::cout << "int: " << i << std::endl;
        }

        //float handling
        if(!floatPossible)
        {
            std::cout << "float: " << "impossible" << std::endl;
        }
        else
        {
            std::cout << "float: " << std::fixed << std::setprecision(1);
            std::cout << f << "f" << std::endl;
        }

        //double handling
        std::cout << "double: " << std::fixed << std::setprecision(1) << d << std::endl;
    }
    
}

static void convertValues(std::string literal, LiteralType type)
{   
    bool charPossible = true;
    bool charDisplayable = true;
    char c = '\0';
    bool intPossible = true;
    int i = 0;
    bool floatPossible = true;
    float f = 0.0f;
    double d = 0.0;
    bool pseudo = false;
    bool hasPlusSign = false;

    //d = strtod(literal.c_str(), NULL);
    switch (type)
    {                
        case CHAR:
        {
            c = literal[0]; // convert it from string to its actual type
            if ((c >= 0 && c <= 31 ) || (c == 127)) //If a conversion to char is not displayable, print an informative message
            {
                charDisplayable = false;
            }
            i = static_cast<int>(c); // then convert it explicitly to the three other data types
            f = static_cast<float>(c);
            d = static_cast<double>(c);
            
            printValues(charPossible, charDisplayable, c, 
                        intPossible, i, floatPossible, f, d, pseudo, hasPlusSign);
            break;
        }
        case INT:
        {
            i = atoi(literal.c_str());

            if ((i > 127) || (i < 0))
            {
                charPossible = false;
            }
            else if ((i >= 0 && i <= 31 ) || (i == 127))
            {
                charDisplayable = false;
            }           
            else
            {
                c = static_cast<char>(i);
            }
            f = static_cast<float>(i);
            d = static_cast<double>(i);
            printValues(charPossible, charDisplayable, c, 
                        intPossible, i, floatPossible, f, d, pseudo, hasPlusSign);
            break;
        }
        case FLOAT:
        {
            f = static_cast<float>(atof(literal.c_str()));
            if ((f > 127) || (f < 0))
            {
                charPossible = false;
            }
            else if ((f >= 0 && f <= 31 ) || (f == 127))
            {
                charDisplayable = false;
            }           
            else
            {
                c = static_cast<char>(f);
            }
            if(f > std::numeric_limits<int>::max() || f < std::numeric_limits<int>::min())         
            {
                intPossible = false;
            }
            else
            {
                i = static_cast<int>(f);
            }
            d = static_cast<double>(f);
            printValues(charPossible, charDisplayable, c, 
                        intPossible, i, floatPossible, f, d, pseudo, hasPlusSign);
            break;
        }
        case DOUBLE:
        {
            d = strtod(literal.c_str(), NULL);

            if ((d > 127) || (d < 0))
            {
                charPossible = false;
            }
            else if ((d >= 0 && d <= 31 ) || (d == 127))
            {
                charDisplayable = false;
            }           
            else
            {
                c = static_cast<char>(d);
            }
            
            if(d > std::numeric_limits<int>::max() || d < std::numeric_limits<int>::min())         
            {
                intPossible = false;
            }
            else
            {
                i = static_cast<int>(d);
            }
            if(d > std::numeric_limits<float>::max() || d < -std::numeric_limits<float>::max())
            {
                floatPossible = false;
            }
            else
            {
                f = static_cast<float>(d);
            }
            printValues(charPossible, charDisplayable, c, 
                        intPossible, i, floatPossible, f, d, pseudo, hasPlusSign);
            break;
        }
        case PSEUDO:
        {
            pseudo = true;
            if(literal[0] == '+')
                hasPlusSign = true;
            d = strtod(literal.c_str(), NULL);
            f = static_cast<float>(d);
            c = ' ';
            i = 0;
            printValues(charPossible, charDisplayable, c, 
                        intPossible, i, floatPossible, f, d, pseudo, hasPlusSign);
            break;
        }
        case INVALID:
        default:
            std::cout << "Conversion impossible: bad format or overflowed value" << std::endl;
            break;
    }
}

void ScalarConverter::convert(const std::string& literal)
{
    LiteralType type = detectType(literal);
    convertValues(literal, type);
}


//float has 7 significant digits total
//double has 15 significant digits total

//for float and double the min() is the smallest representable value sth like: 0,000...00017
