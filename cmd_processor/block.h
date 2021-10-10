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

  State currentState(const std::string & token);
  void flush();
  void output() const;
  unsigned long long getTimePoint() const;
  
  struct CmdInfo
  {
    std::string m_command;
    unsigned long long m_tp = 0;
  };

  std::vector<CmdInfo> m_aCommands;

private:
  void reset();
  void outFile(std::string fileName) const;
  
  int m_startDynCmdCount = 0;
};
