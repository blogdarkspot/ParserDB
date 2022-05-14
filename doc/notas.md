# Constituitnte 
    É um grupo de plavras que pode se comportar como uma unidade única.

# Gramatica livre de contexto
    Consiste em um grupo de regras 



    # Algorithimo

    dado um set de roles  X  onde  as roles são definidas por  (S -> N V, S-> V, S-> N, N-> X Y,, V, P)

    e de entrada temos uma cadeia de verbetes a saída é definida por um set de roles que define esta 
    cadeia de verbetes:

    primeiro passo verificar se o verbete faz parte de um nucléo da constintuite .
    segundo salve o range do lado esquerdo, pois uma constintuinte pode ser composta por outras
    constintuintes.
    também salve o nucleo atual
    terceiro procure a lista de roles baseadas na raiz atual;

    verbete {
        list<string> class;
        list<bool> roots;
        string desc;
    }

    roles_desc
    {
        int distance_from_root;
        string desc;
    }

    roles
    {
         queue<roles_desc>  next;
         bool is_valid;
         bool is_finished;
    }
    map<string, list<roles>> roles;

    input  (list<verbetes> v) => list<roles>

        int lpos = 0;
        string root;

        for (int i = 0; i < v.size(); ++i)
        {
                for(int j = 0; j < v[i].roots.size(); ++j)
                {
                    if(v[i].root(j))
                    {
                        
                    }
                }
                
        }



<t>
                   //party name Upper Case       categoria      Genero   Singular        Top Level np          tree lvl
PT      [PT]      <party>         <*>                    PROP            M/F          S                      @NPHR                  #1->0
                    //first part split
em      [em] <sam-> PRP @ADVL>  #2->0
o       [o] <-sam> <artd> DET M S @>N  #3->4
governo         [governo] <HH> N M S @P<  #4->2
</t>