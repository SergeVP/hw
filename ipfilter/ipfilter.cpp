// ipfilter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream> 

#include <algorithm>
#include <set>
#include <sstream>
#include <tuple>

struct Address
{
  bool operator < (const Address & other) const
  {
    return std::tie(m_1, m_2, m_3, m_4) < std::tie(other.m_1, other.m_2, other.m_3, other.m_4);
  };

  bool isValid() const
  {
    return m_1 >= 0 && m_2 >= 0 && m_3 >= 0 && m_4 >= 0 && m_1 < 256 && m_2 < 256 && m_3 < 256 && m_4 < 256;
  }

  int m_1 = 0;
  int m_2 = 0;
  int m_3 = 0;
  int m_4 = 0;
};

Address convertAddressString(const std::string & stringIP)
{
  Address address;
  std::stringstream sstream(stringIP);
  char dummy; 
  sstream >> address.m_1 >> dummy >> address.m_2 >> dummy >> address.m_3 >> dummy >> address.m_4;

  if (address.isValid())
  {
    return address;
  }
  else
  {
    throw std::string{ "Invalid input string -- terminated." };
  }
}

std::multiset<Address> readInput()
{
  std::multiset<Address> aAddressesSet;

  std::string dummy;
  while (!std::cin.eof())
  {
    std::string stringIP;
    std::cin >> stringIP >> dummy >> dummy;
    if (stringIP.empty())
    {
      continue;
    }
    if (stringIP.size() > 6)
    {
      aAddressesSet.insert(convertAddressString(stringIP));
    }
    else
    {
      throw std::string{ "Invalid input string -- terminated." };
    }
  }

  return aAddressesSet;
}

int main()
{
  try
  {
    const auto & aAddressesSet = readInput();

    // conditions
    auto any = [](const Address &) { return true; };
    auto first1 = [](const Address & adr) { return adr.m_1 == 1; };
    auto first46second70 = [](const Address & adr) { return adr.m_1 == 46 && adr.m_2 == 70; };
    auto any46 = [](const Address & adr) { return ((adr.m_1 == 46) || (adr.m_2 == 46) || (adr.m_3 == 46) || (adr.m_4 == 46)); };

    // helper
    auto writer = [&aAddressesSet](bool cond(const Address &))
    {
      std::for_each(aAddressesSet.rbegin(), aAddressesSet.rend(), [&](const Address & address)
      {
        if (cond(address))
        {
          std::cout << address.m_1 << "." << address.m_2 << "." << address.m_3 << "." << address.m_4 << std::endl;
        }
      });
    };

    // output
    writer(any);
    writer(first1);
    writer(first46second70);
    writer(any46);
  }

  catch (std::string message)
  {
    std::cout << message.c_str() << std::endl;
    exit(1);
  }
}
