// Uvod u umjetnu inteligenciju - 2. labos: PDA.
// Kristijan Burnik
// -----------------------------------------------------------------------------
// Trenutni nedostaci
// Nema provjere ispravnosti izraza:
// * Program se ruši ako je stog prazan i pokuša se skidati sa stoga.
// * Moguće je da na stogu ostani simboli nakon evaluacije izraza.
// - Sve gore navedeno je lako razriješivo na nekoliko načina, ali
//   nije definirano zadatakom.
#include <iostream>
#include <stack>
#include <set>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

// Operatori.
const string OP_NOT = "NOT";
const string OP_AND = "AND";
const string OP_OR = "OR";

// Konstante.
const string ATOM_TRUE = "True";
const string ATOM_FALSE = "False";

// Konverzija string u boolean.
const bool stringToBool(const string& s) {
  return s == ATOM_TRUE;
}

// Konverzija boolean u string.
const string boolToString(const bool v) {
  return v ? ATOM_TRUE : ATOM_FALSE;
}

// Klasa za evaluaciju logičkih izraza.
class Eval {
 public:
   // Konstruktor prima skup istinitih simbola
   Eval(const set<string> facts) : _facts(facts) {}

   // Izračunava istinitost izraza.
   const bool evaluate(const string& expression) {
    stringstream ss;
    ss << expression;
    vector<string> tokens{istream_iterator<string>{ss},
                          istream_iterator<string>{}};
    clear();
    for (int i = 0; i < tokens.size(); i++) {
      const string token = tokens[i];
      // Binarne operacije.
      if (_binary_ops.count(token)) {
        const string first = pop();
        const string second = pop();
        push(compute(token, first, second));
      // Unarne operacije
      } else if (_unary_ops.count(token)) {
        const string first = pop();
        push(compute(token, first));
      // Simboli.
      } else {
        push(boolToString(_facts.count(token)));
      }
    }
    return stringToBool(pop());
   }

 private:
  // Čisti stog.
  void clear() {
    while(!_stack.empty()) _stack.pop();
  }

  // Vraća vrh stoga i uklanja element.
  const string pop() {
    const string result = _stack.top();
    _stack.pop();
    return result;
  }

  // Dodaje na vrh stoga.
  const void push(const string& value) {
    _stack.push(value);
  }

  // Izračunava rezultat unarne operaciju.
  const string compute(const string op, const string first) {
    return compute(op, first, "");
  }

  // Izračunava rezultate binarne operacije.
  string compute(const string op, const string first, const string second) {
    const bool firstVal = stringToBool(first);
    const bool secondVal = stringToBool(second);
    if (op == OP_AND) {
      return boolToString(firstVal && secondVal);
    } else if (op == OP_OR) {
      return boolToString(firstVal || secondVal);
    } else if (op == OP_NOT) {
      return boolToString(!firstVal);
    } else {
      cerr << "Neocekivana operacija:" << op << endl;
      return ATOM_FALSE;
    }
  }

  // Stog simbola i operacija.
  stack<string> _stack;

  // Istiniti simboli.
  const set<string> _facts;

  // Unarne operacije.
  const set<string> _unary_ops = {OP_NOT};

  // Binarne operacije.
  const set<string> _binary_ops = {OP_AND, OP_OR};
};

int main() {
  int n;
  cin >> n; // Broj istinitih tvrdnji.

  set<string> facts;
  for (int i = 0; i < n; i++) {
    string s;
    cin >> s;
    facts.insert(s);
  }

  string s;
  while (getline(cin, s)) {
    if (s == "")
      continue;
    const bool result = Eval(facts).evaluate(s);
    cout << boolToString(result) << endl;
  }

  return 0;
}
