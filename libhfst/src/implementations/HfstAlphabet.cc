#include "HfstAlphabet.h"

namespace hfst {
  namespace implementations {
    
    HfstAlphabet::HfstAlphabet() {
      add("@_EPSILON_SYMBOL_@",0);
      add("@_UNKNOWN_SYMBOL_@",1);
      add("@_IDENTITY_SYMBOL_@",2);
    }
    
    HfstAlphabet::~HfstAlphabet() {
      char **s=new char*[cm.size()];
      pairs.clear();
      sm.clear();
      
      size_t i, n=0;
      for( CharMap::iterator it=cm.begin(); it!=cm.end(); it++ )
	s[n++] = it->second;
      cm.clear();
      
      for( i=0; i<n; i++ )
	free(s[i]);
      delete[] s;
    }
    
    HfstAlphabet::const_iterator HfstAlphabet::begin() const { return pairs.begin(); }
    HfstAlphabet::const_iterator HfstAlphabet::end() const { return pairs.end(); };
    size_t HfstAlphabet::size() const { return pairs.size(); };
    
    //bool HfstAlphabet::contains_special_symbols(StringPair sp);  TODO!
    
    void HfstAlphabet::insert(NumberPair sp) { /* check special symbols */ pairs.insert(sp); };  // TODO!
    void HfstAlphabet::clear_pairs() { pairs.clear(); };
    HfstAlphabet::CharMap HfstAlphabet::get_char_map() { return cm; };

    void HfstAlphabet::add( const char *symbol, unsigned int c ) {
      char *s = HfstBasic::fst_strdup(symbol);
      cm[c] = s;
      sm[s] = c;
    }
    
    int HfstAlphabet::symbol2code( const char * s ) const {
      SymbolMap::const_iterator p = sm.find(s);
      if (p != sm.end()) return p->second;
      return EOF;	
    }
    
    const char *HfstAlphabet::code2symbol( unsigned int c ) const {
      CharMap::const_iterator p=cm.find(c);
      if (p == cm.end())
	return NULL;
      else
	return p->second;
    }
    
    unsigned int HfstAlphabet::add_symbol(const char * symbol) {
      if (sm.find(symbol) != sm.end())
	return sm[symbol];
      
      // assign the symbol to some unused character
      for(unsigned int i=1; i!=0; i++)
	if (cm.find(i) == cm.end()) {
	  add(symbol, i);
	  return i;
	}
      
      throw "Error: too many symbols in transducer definition";
    }
    
    void HfstAlphabet::add_symbol( const char *symbol, unsigned int c )

    {
      // check whether the symbol was previously defined
      int sc=symbol2code(symbol);
      if (sc != EOF) {
	if ((unsigned int)sc == c)
	  return;
	
	if (strlen(symbol) < 60) {
	  static char message[100];
	  sprintf(message, "Error: reinserting symbol '%s' in alphabet with incompatible character value %u %u", symbol, (unsigned)sc, (unsigned)c);
	  throw message;
	}
	else
	  throw "reinserting symbol in alphabet with incompatible character value";
      }
      
      // check whether the character is already in use
      const char *s=code2symbol(c);
      if (s == NULL)
	add(symbol, c);
      else {
	if (strcmp(s, symbol) != 0) {
	  static char message[100];
	  if (strlen(symbol) < 70)
	    sprintf(message,"Error: defining symbol %s as character %d (previously defined as %s)", symbol, (unsigned)c, s);
	  else
	    sprintf(message,"Error: defining a (very long) symbol with previously used character");
	  throw message;
	}
      }
    }

    void HfstAlphabet::complement( std::vector<unsigned int> &sym ) {
      std::vector<unsigned int> result;
      for( CharMap::const_iterator it=cm.begin(); it!=cm.end(); it++ ) {
	unsigned int c = it->first;
	if (c != 0) { // Label::epsilon
	  size_t i;
	  for( i=0; i<sym.size(); i++ )
	    if (sym[i] == c)
	      break;
	  if (i == sym.size())
	    result.push_back(c);
	}
      }
      sym.swap(result);
    }
    
  }
}
