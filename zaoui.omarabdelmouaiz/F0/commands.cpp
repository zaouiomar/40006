#include "commands.hpp"
#include <fstream>
#include <algorithm>
#include <string>
#include <functional>
#include <numeric>
#include <iomanip>
#include <sstream>
#include <map>

namespace
{
  using constWord  = const std::pair< std::string, zaoui::Xrefs >;
  using Word  = std::pair< const std::string, zaoui::Xrefs >;

  bool cmpMaxWordLen(constWord & a, constWord & b)
  {
    return a.first.size() < b.first.size();
  }

  struct PrintWordPos
  {
    std::ostream & out;
    void operator()(const std::pair< size_t, size_t > & pos) const
    {
      out << " (" << pos.first << ", " << pos.second << ')';
    }
  };

  struct PrintWords
  {
    std::ostream & out;
    size_t width;
    void operator()(constWord & word) const
    {
      out << std::left << std::setw(width) << word.first;
      std::for_each(word.second.cbegin(), word.second.cend(), PrintWordPos{out});
      out << '\n';
    }
  };

  std::pair< zaoui::WordPos, std::string > createWordPair(const zaoui::WordPos & pos, const std::string & word)
  {
    return std::make_pair(pos, word);
  }

  struct Accumulator
  {
    std::string result;
    zaoui::WordPos prev;
    bool first = true;
    Accumulator(const std::string & initial):
      result(initial)
    {}
  };

  bool compareWordPositions(const std::pair< zaoui::WordPos, std::string > & a,
  const std::pair< zaoui::WordPos, std::string > & b)
{
  if (a.first.first != b.first.first) {
    return a.first.first < b.first.first;
  }
  return a.first.second < b.first.second;
}

  Accumulator accumulateWords(Accumulator acc, const std::pair< zaoui::WordPos, std::string > & current)
  {
    if (acc.first)
    {
      acc.result += current.second;
      acc.first = false;
    }
    else
    {
      if (acc.prev.first != current.first.first)
      {
        acc.result += '\n';
      }
      else
      {
        acc.result += ' ';
      }
      acc.result += current.second;
    }
    acc.prev = current.first;
    return acc;
  }

  void supRecText(constWord & word, std::vector< std::pair< zaoui::WordPos, std::string > > & sortedWords)
  {
    auto b = word.second.begin();
    auto e = word.second.end();
    using namespace std::placeholders;
    std::transform(b, e, std::back_inserter(sortedWords), std::bind(createWordPair, _1, word.first));
  }

  std::string reconstructText(const zaoui::Text & text)
  {
    std::vector< std::pair< zaoui::WordPos, std::string > > sortedWords;
    using namespace std::placeholders;
    std::for_each(text.cbegin(), text.cend(), std::bind(supRecText, _1, std::ref(sortedWords)));
    std::sort(sortedWords.begin(), sortedWords.end(), compareWordPositions);
    if (sortedWords.empty())
    {
      return "";
    }
    Accumulator initialValue("");
    Accumulator result = std::accumulate(sortedWords.begin(), sortedWords.end(), initialValue, accumulateWords);
    return result.result;
  }


  bool cmpMaxLineNumWord(const std::pair< size_t, size_t > & a, const std::pair< size_t, size_t > & b)
  {
    return a.first < b.first;
  }

  size_t getMaxLineNumWord(constWord & word)
  {
    zaoui::Xrefs xrefs = word.second;
    if (xrefs.empty())
    {
      return 0;
    }
    auto maxNumIt = std::max_element(xrefs.cbegin(), xrefs.cend(), cmpMaxLineNumWord);
    return maxNumIt->first;
  }

  size_t getMaxLineNum(const zaoui::Text & text)
  {
    std::vector< size_t > lineNum;
    std::transform(text.cbegin(), text.cend(), std::back_inserter(lineNum), getMaxLineNumWord);
    return *std::max_element(lineNum.cbegin(), lineNum.cend());
  }

 bool cmpBetween(const zaoui::WordPos & pos, size_t begin, size_t end)
{
  return pos.first >= begin && pos.first <= end;
}

  void subExtrSubstr(constWord & word, zaoui::Text & result, size_t begin, size_t end)
  {
    zaoui::Xrefs newXrefs;
    auto b = word.second.begin();
    auto e = word.second.end();
    using namespace std::placeholders;
    std::copy_if(b, e, std::back_inserter(newXrefs), std::bind(cmpBetween, _1, begin, end));
    if (!newXrefs.empty())
    {
      result[word.first] = newXrefs;
    }
  }

  zaoui::Text extractSubstring(const zaoui::Text & text, size_t begin, size_t end)
  {
    zaoui::Text result;
    using namespace std::placeholders;
    std::for_each(text.cbegin(), text.cend(), std::bind(subExtrSubstr, _1, std::ref(result), begin, end));
    return result;
  }

  zaoui::WordPos downLenNum(const zaoui::WordPos & pos, size_t begin, size_t end)
  {
    if (pos.first >= end)
    {
      return {pos.first - (end - begin + 1), pos.second};
    }
    return pos;
  }

  zaoui::WordPos reverseLenNum(const zaoui::WordPos & pos, size_t maxLine)
  {
    return {maxLine - pos.first + 1, pos.second};
  }

  void subRemoveSubstr(Word & word, size_t begin, size_t end)
  {
    using namespace std::placeholders;
    auto iter = std::remove_if(word.second.begin(), word.second.end(), std::bind(cmpBetween, _1, begin, end));
    word.second.erase(iter, word.second.end());
  }

  void subDownLenNum(Word & word, size_t begin, size_t end)
  {
    auto b = word.second.begin();
    auto e = word.second.end();
    using namespace std::placeholders;
    std::transform(b, e, b, std::bind(downLenNum, _1, begin, end));
  }

  void removeSubstring(zaoui::Text & text, size_t begin, size_t end)
  {
    using namespace std::placeholders;
    std::for_each(text.begin(), text.end(), std::bind(subRemoveSubstr, _1, begin, end));
    std::for_each(text.begin(), text.end(), std::bind(subDownLenNum, _1, begin, end));
  }
  void insertTextTo(zaoui::Text & text1, const zaoui::Text & text2, size_t n, size_t begin, size_t end)
{

  zaoui::Text temp = extractSubstring(text2, begin, end);

  size_t linesToInsert = end - begin + 1;
  std::for_each(text1.begin(), text1.end(),
    [n, linesToInsert](auto& wordEntry) {
      auto& positions = wordEntry.second;
      std::for_each(positions.begin(), positions.end(),
        [n, linesToInsert](auto& pos) {
          if (pos.first >= n) {
            pos.first += linesToInsert;
          }
        });
    });

  std::for_each(temp.cbegin(), temp.cend(),
    [&text1, n, begin](const auto& wordEntry) {
      const std::string& word = wordEntry.first;
      std::for_each(wordEntry.second.cbegin(), wordEntry.second.cend(),
        [&text1, &word, n, begin](const auto& pos) {

          size_t newLine = n + (pos.first - begin);
          text1[word].push_back({newLine, pos.second});
        });
    });
}

  void subInvertLines(Word & word, size_t lineNum)
  {
    auto b = word.second.begin();
    auto e = word.second.end();
    using namespace std::placeholders;
    std::transform(b, e, b, std::bind(reverseLenNum, _1, lineNum));
  }
}

void zaoui::generateLinks(std::istream & in, Texts & texts)
{
  std::string textName, fileName;
  in >> textName >> fileName;
  std::ifstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  if (texts.find(textName) != texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  Text text{};
  std::string lineStr;
  size_t lineNum = 0;
  while (std::getline(file, lineStr))
  {
    ++lineNum;
    if (lineStr.empty()) continue;
    std::istringstream lineStream(lineStr);
    std::string word;
    size_t wordNum = 0;

    while (lineStream >> word)
    {
      ++wordNum;

      text[word].push_back({lineNum, wordNum});
    }
  }
  texts[textName] = std::move(text);
}

void zaoui::removeLinks(std::istream & in, Texts & texts)
{
  std::string textName;
  in >> textName;
  if (texts.find(textName) == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  texts.erase(textName);
}

void zaoui::printLinks(std::istream & in, std::ostream & out, const Texts & texts)
{
  std::string textName;
  in >> textName;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  const Text & text = it->second;
  auto maxWordIt = std::max_element(text.cbegin(), text.cend(), cmpMaxWordLen);
  size_t maxWordLen = maxWordIt->first.length() + 2;
  std::for_each(text.cbegin(), text.cend(), PrintWords{out, maxWordLen});
  out << "\n";
}

void zaoui::printText(std::istream & in, std::ostream & out, const Texts & texts)
{
  std::string textName;
  in >> textName;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  const Text & text = it->second;
  std::string reconstructed = reconstructText(text);
  out << reconstructed << '\n';
  out << "\n";
}

void zaoui::printTextInFile(std::istream & in, const Texts & texts)
{
  std::string fileName;
  in >> fileName;
  std::ofstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  printText(in, file, texts);
}

void zaoui::mergeTexts(std::istream & in, Texts & texts)
{
  std::string newText, textName1, textName2;
  in >> newText >> textName1 >> textName2;
  auto it1 = texts.find(textName1);
  auto it2 = texts.find(textName2);
  auto it = texts.find(newText);
  if (it1 == texts.end() || it2 == texts.end() || it != texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  const Text & text1 = it1->second;
  const Text & text2 = it2->second;
  Text temp = text1;

  size_t insertionPoint = getMaxLineNum(temp) + 1;

  insertTextTo(temp, text2, insertionPoint, 1, getMaxLineNum(text2));
  texts[newText] = std::move(temp);
}

void zaoui::insertText(std::istream & in, Texts & texts)
{
    std::string textName1, textName2;
    size_t num = 0, begin = 0, end = 0;
    in >> textName1 >> num >> textName2 >> begin >> end;
    auto it1 = texts.find(textName1);
    auto it2 = texts.find(textName2);
    if (it1 == texts.end() || it2 == texts.end()) {
        throw std::runtime_error("<INVALID COMMAND>");
    }

    Text & text1 = it1->second;
    const Text & text2 = it2->second;

    size_t maxLine1 = getMaxLineNum(text1);
    size_t maxLine2 = getMaxLineNum(text2);

    if (num < 1 || num > maxLine1 + 1 ||
        begin < 1 || begin > maxLine2 ||
        end < begin || end > maxLine2) {
        throw std::runtime_error("<INVALID COMMAND>");
    }

    insertTextTo(text1, text2, num, begin, end);
}

void zaoui::removeLines(std::istream & in, Texts & texts)
{
  std::string textName;
  size_t begin = 0, end = 0;
  in >> textName >> begin >> end;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text = it->second;
  size_t maxLine = getMaxLineNum(text);

  if (begin < 1 || begin > maxLine ||
      end < begin || end > maxLine)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  removeSubstring(text, begin, end);
}

void zaoui::moveText(std::istream & in, Texts & texts)
{
  std::string textName1, textName2;
  size_t num = 0, begin = 0, end = 0;
  in >> textName1 >> num >> textName2 >> begin >> end;
  auto it1 = texts.find(textName1);
  auto it2 = texts.find(textName2);
  if (it1 == texts.end() || it2 == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  Text & text1 = it1->second;
  Text & text2 = it2->second;

  size_t maxLine1 = getMaxLineNum(text1);
  size_t maxLine2 = getMaxLineNum(text2);


  if (num < 1 || num > maxLine1 + 1 ||
      begin < 1 || begin > maxLine2 ||
      end < begin || end > maxLine2)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  insertTextTo(text1, text2, num, begin, end);
  removeSubstring(text2, begin, end);
}

void zaoui::splitTexts(std::istream & in, Texts & texts)
{
  std::string textName, newText1, newText2;
  size_t num = 0;
  in >> textName >> num >> newText1 >> newText2;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text = it->second;
  size_t end = getMaxLineNum(text);
  if (num > end)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text newText = extractSubstring(text, num, end);
  removeSubstring(text, num, end);
  texts[newText1] = std::move(text);
  texts[newText2] = std::move(newText);
  texts.erase(textName);
}

void zaoui::invertLines(std::istream & in, Texts & texts)
{
  std::string textName;
  in >> textName;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text = it->second;
  size_t maxLine = getMaxLineNum(text);
  using namespace std::placeholders;
  std::for_each(text.begin(), text.end(), std::bind(subInvertLines, _1, maxLine));
}

void zaoui::invertWords(std::istream & in, Texts & texts)
{
  std::string textName;
  in >> textName;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text = it->second;

  std::map<size_t, size_t> lineMaxPos;
  std::for_each(text.cbegin(), text.cend(),
    [&lineMaxPos](const auto& wordEntry) {
      std::for_each(wordEntry.second.cbegin(), wordEntry.second.cend(),
        [&lineMaxPos](const auto& pos) {
          if (pos.second > lineMaxPos[pos.first]) {
            lineMaxPos[pos.first] = pos.second;
          }
        });
    });


  std::for_each(text.begin(), text.end(),
    [&lineMaxPos](auto& wordEntry) {
      auto& positions = wordEntry.second;
      std::for_each(positions.begin(), positions.end(),
        [&lineMaxPos](auto& pos) {
          size_t maxPos = lineMaxPos[pos.first];
          if (maxPos > 0) {
            pos.second = maxPos - pos.second + 1;
          }
        });
    });
}

void zaoui::replaceWord(std::istream & in, Texts & texts)
{
  std::string textName, oldWord, newWord;
  in >> textName >> oldWord >> newWord;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<TEXT NOT FOUND>");
  }
  Text & text = it->second;


  if (oldWord == newWord) {
    return;
  }

  auto oldWordIt = text.find(oldWord);
  if (oldWordIt == text.end())
  {
    throw std::runtime_error("<OLD WORD NOT FOUND>");
  }


  auto newWordIt = text.find(newWord);
  if (newWordIt != text.end()) {

    newWordIt->second.insert(newWordIt->second.end(),
                            oldWordIt->second.begin(),
                            oldWordIt->second.end());
  } else {

    text[newWord] = std::move(oldWordIt->second);
  }


  text.erase(oldWordIt);
}

void zaoui::printHelp(std::ostream & out)
{
  out << "Available commands:\n";
  out << "generatelinks <text name> <text file> - generate new links from file\n";
  out << "removelinks <text name> - delete text\n";
  out << "printlinks <text name> - print words from text with xref\n";
  out << "printtext <text name> - print text\n";
  out << "printtextinfile <text file> <text name> - print text in file\n";
  out << "merge <text result name> <text1 name> <text2 name> - merge two texts\n";
  out << "insert <text1 name> <num> <text2 name> <begin> <end> - insert part of the text into another\n";
  out << "removelines <text name> <begin> <end> - delete part of the text\n";
  out << "move <text1 name> <num> <text2 name> <begin> <end> - move part of the text into another\n";
  out << "split <text name> <num> <new text1 name> <new text2 name> - split the text into two texts\n";
  out << "invertlines <text name> - reverse the order of lines in the text\n";
  out << "invertwords <text name> - reverse the order of words in each line in the text\n";
  out << "replaceword <text name> <old word> <new word> - replace a word in the text with a new one \n";
  out << "--help - show this help\n";
}
