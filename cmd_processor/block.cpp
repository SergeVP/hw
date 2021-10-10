#include "block.h"

#include <chrono>
#include <iostream>

#define NDEBUG  // comment this line to enable assert()
#include <cassert>
#define ASSERT_MSG(exp, msg) assert(((void)msg, exp))


Block::State Block::currentState(const std::string & token)
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
      // unexpected -- seems that the input is not fully correct; probably, should be reported
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

void Block::flush()
{
  output();
  reset();
}

void Block::output() const
{
  if (m_aCommands.empty())
  {
    ASSERT_MSG(!m_aCommands.empty(), "Improper output of an empty command block");
    return;
  }

  auto timeStamp = std::to_string(m_aCommands[0].m_tp);
  outFile(timeStamp);

  for (const auto & command : m_aCommands)
  {
    std::cout << command.m_command.c_str();
  }
}

unsigned long long Block::getTimePoint() const
{
  auto st = std::chrono::system_clock::now();
  return std::chrono::duration_cast<std::chrono::seconds>(st.time_since_epoch()).count();
};

void Block::reset()
{
  m_aCommands.clear();
};

void Block::outFile(std::string fileName) const
{
  fileName = "bulk" + fileName + ".log";

  FILE * pFile = fopen(fileName.c_str(), "w+");

  for (const auto & cmd : m_aCommands)
  {
    fprintf(pFile, "%s", cmd.m_command.c_str());
  }

  fflush(pFile);
  fclose(pFile);
}
