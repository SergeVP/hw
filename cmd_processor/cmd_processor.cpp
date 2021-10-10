// cmd_processor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "block.h"


class StaticBlock : public Block
{
public:
  StaticBlock(int n) : Block(), m_n(n) {};
  ~StaticBlock() = default;

  void add(const std::string & token) override
  {
    const auto & state = currentState(token);

    if (state == State::eDynStarted)
    {
      flush();
      m_onDuty = false;
    }
    else if (state == State::eDynEdned)
    {
      m_onDuty = true;
    }

    if (m_onDuty && (state == State::eNormal))
    {
      m_aCommands.push_back(CmdInfo{ token, getTimePoint() });
    }

    doOnComplete();
  }

  void onEOF() const override
  {
    output();
  }

private:
  void doOnComplete()
  {
    if (m_aCommands.size() == static_cast<size_t>(m_n))
    {
      flush();
    }
  }

  unsigned m_n = 0;
  bool m_onDuty = true;
};


class DynBlock : public Block
{
public:
  DynBlock() = default;
  ~DynBlock() = default;

  void add(const std::string & token) override
  {
    const auto & state = currentState(token);

    if (state == State::eDynStarted)
    {
      m_onDuty = true;
    }
    else if (state == State::eDynEdned)
    {
      m_onDuty = false;
      flush();
    }

    if (m_onDuty && (state == State::eNormal))
    {
      m_aCommands.push_back(CmdInfo{ token, getTimePoint() });
    }
  }

  void onEOF() const override
  {
  }

private:
  bool m_onDuty = false;
};


int main(int argc, char *argv[])
{
  auto staticBlockLength = (argc > 1) ? atoi(argv[1]) : 3;

  StaticBlock staticBlock(staticBlockLength);
  DynBlock dynamicBlock;

  try
  {
    for (;;)
    {
      std::string s;

      std::cin >> s;
      if (s.empty())
      {
        throw (false);
      }

      staticBlock.add(s);
      dynamicBlock.add(s);
      /*!!!auto temp = time(NULL) + 2;
      while (temp > time(NULL)); */
    }
  }

  catch (...)
  {
    staticBlock.onEOF();
    dynamicBlock.onEOF();
  }
}

