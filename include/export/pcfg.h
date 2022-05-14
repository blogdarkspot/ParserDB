
class PCFG
{
    struct _implPCFG;

  public:
    PCFG();

    ~PCFG();

    void add_rule(wchar_t *lhs, wchar_t *rhs);

    void add_terminal(wchar_t *lhs, wchar_t *rhs);

    void parser(wchar_t *s, wchar_t **matrix);


  private:
    _implPCFG *_impl;
};

