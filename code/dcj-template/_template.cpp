// Template available at "github.com/geocklam/jam" version 20160524.
#include <bits/stdc++.h>
using namespace std;
#include <message.h>
namespace jam {
  typedef long long LL;
  const int NUM = NumberOfNodes();
  const int ID = MyNodeId();
  LL srandLL(unsigned int seed) {
    srand(seed);
    return seed;
  }
  LL randLL() {
    return (rand() % 32768LL * 32768LL + rand() % 32768LL) * 32768LL
        + rand() % 32768LL;
  }
  struct Din {
    int source = -1;
    Din& operator()(int source) {
      this->source = Receive(source);
      return *this;
    }
    Din& operator>>(char& x) {
      x = GetChar(source);
      return *this;
    }
    Din& operator>>(int& x) {
      x = GetInt(source);
      return *this;
    }
    Din& operator>>(LL& x) {
      x = GetLL(source);
      return *this;
    }
  } din;
  struct Dend {} dend;
  struct Dout {
    int target = -1;
    Dout& operator()(int target) {
      this->target = target;
      return *this;
    }
    Dout& operator<<(Dend& dend) {
      Send(target);
      return *this;
    }
    Dout& operator<<(char x) {
      PutChar(target, x);
      return *this;
    }
    Dout& operator<<(int x) {
      PutInt(target, x);
      return *this;
    }
    Dout& operator<<(LL x) {
      PutLL(target, x);
      return *this;
    }
  } dout;
  struct Dsync {
    struct BaseRef {
      virtual void put(Dout& dout) = 0;
      virtual ~BaseRef() {};
    };
    template <class T> struct Ref : BaseRef {
      T& x;
      Ref(T& x) : x(x) {}
      virtual void put(Dout& dout) {
        dout << x;
      }
      virtual ~Ref() {}
    };
    int source = -1;
    vector< unique_ptr<BaseRef> > buffer;
    Dsync& operator()(int source) {
      this->source = source;
      if (ID != source) {
        din(source);
      }
      return *this;
    }
    Dsync& operator<<(Dend& x) {
      if (ID == source) {
        for (int k=0; k<NUM; ++k) {
          if (k != ID) {
            dout(k);
            for (auto& v : buffer) {
              v->put(dout);
            }
            dout << dend;
          }
        }
        buffer.clear();
      }
      return *this;
    }
    template <class T> Dsync& operator<<(T& x) {
      if (ID == source) {
        buffer.push_back(unique_ptr<BaseRef>(new Ref<T>(x)));
      } else {
        din >> x;
      }
      return *this;
    }
  } dsync;
  template <class T, class Func> void merge(int target, T& x, const Func& f) {
    dout(target) << x << dend;
    if (ID == target) {
      din(0) >> x;
      for (int k=1; k<NUM; ++k) {
        T y;
        din(k) >> y;
        x = f(x,y);
      }
    }
  }
} // namespace jam
using namespace jam;

LL SEED = srandLL(ID);
// LL N = GetN();
// LL a = N * ID / NUM;
// LL b = N * (ID + 1) / NUM;

struct Msg {};
Din& operator>>(Din& din, Msg& x) {
  return din;
}
Dout& operator<<(Dout& dout, const Msg& x) {
  return dout;
}
Msg& combine(Msg& x, Msg& y) {
  return x;
}

int main() {

  if (ID == 0) {
    LL ans = 0;
    cout << ans << endl;
  }
  return 0;
}
