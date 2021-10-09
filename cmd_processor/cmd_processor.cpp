// cmd_processor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <chrono>
#include <vector>
#include <string>

class Block
{
public:
  Block() = default;
  virtual ~Block() = default;

  virtual void add(const std::string & token) = 0;
  virtual void onEOF() const = 0;

protected:
  enum class State
  {
    eNormal     = 0,
    eDynStarted = 1,
    eDynEdned   = 2,
    eDynNotChanged
  };

  State currentState(const std::string & token)
  {
    if (token == std::string{ '{' })
    {
      ++m_startDynCmdCount;
    }
   else if (token == std::string{ '}' })
    {
      --m_startDynCmdCount;
      if (m_startDynCmdCount < 0)
      {
        //!!! assert
        m_startDynCmdCount = 0;
      }
    }
    else
    {
      return State::eNormal;
    }

    switch (m_startDynCmdCount)
    {
    case 1: 
      return State::eDynStarted;
    case 0:
      return State::eDynEdned;
    default:
      return State::eDynNotChanged;
    }
  }

  void flush()
  {
    output();
    reset();
  }

  void output() const
  {
    if (m_aCommands.empty())
    {
      //!!! assert
      return;
    }

    auto timeStamp = std::to_string(m_aCommands[0].m_tp);
    outFile(timeStamp);

    for (const auto & command : m_aCommands)
    {
      std::cout << command.m_command.c_str();
    }
  }

  unsigned long long getTimePoint() const
  {
    auto st = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(st.time_since_epoch()).count();
  };

  struct CmdInfo
  {
    std::string m_command;
    unsigned long long m_tp = 0;
  };

  std::vector<CmdInfo> m_aCommands;

private:
  void reset()
  {
    m_aCommands.clear();
  };

  void outFile(std::string fileName) const
  {
    fileName = "bulk" + fileName + ".log";
    
    FILE * pFile = fopen(fileName.c_str(), "w+");
    /*auto err = fopen_s(&pFile, fileName.c_str(), "w+");
    if (err != 0)
    {
      //!! assert
      return;
    }*/

    for (const auto & cmd : m_aCommands)
    {
      fprintf(pFile, "%s", cmd.m_command.c_str());
    }

    fclose(pFile);
  }

  int m_startDynCmdCount = 0;
};


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

