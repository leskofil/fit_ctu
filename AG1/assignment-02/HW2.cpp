#include <iostream>
#include <string>
#include <utility>

class Node;

class CPolitician {
public:
    CPolitician() = default;

    CPolitician(std::string name, uint32_t popularity, uint8_t gender, uint32_t partyID, uint32_t ID, uint64_t time) : m_Name(
            std::move(name)),
                                                                                                                       m_Popularity(
                                                                                                                               popularity),
                                                                                                                       m_Gender(
                                                                                                                               gender),
                                                                                                                       m_PartyID(
                                                                                                                               partyID),
                                                                                                                       m_ID(ID), m_time(time) {}

    uint32_t getPopularity() const { return m_Popularity; }

    uint64_t getMTime() const {
        return m_time;
    }

    void setMPartyId(uint32_t mPartyId) {
        m_PartyID = mPartyId;
    }

    void setMNode(Node *mNode) {
        m_Node = mNode;
    }

    void setMTime(uint64_t mTime) {
        m_time = mTime;
    }

    Node *getMNode() const {
        return m_Node;
    }

    void setMPopularity(uint32_t mPopularity) { m_Popularity = mPopularity; }

    uint32_t getMPartyId() const { return m_PartyID; }

    const std::string &getMName() const {
        return m_Name;
    }

    uint32_t getMId() const {
        return m_ID;
    }

    uint8_t getMGender() const {
        return m_Gender;
    }

    Node *m_Node{};

private:
    std::string m_Name;
    uint32_t m_Popularity{};
    uint8_t m_Gender{};
    uint32_t m_PartyID{};
    uint32_t m_ID{};
    uint64_t m_time{};
};

class Node {
public:
    Node() = default;

    explicit Node(CPolitician *p) : val(p) {}

    ~Node() { delete val; }

    CPolitician *val{};
    int degree{};
    Node *parent{}, *child{}, *sibling{};
};

class CBinomialHeap {
private:
    Node *root{};
    Node *max{};
public:
    CBinomialHeap() = default;

    explicit CBinomialHeap(Node *n) : root(n), max(n) {}

    //CBinomialHeap(Node *n, Node *m) : root(n), max(m) {}

    static CBinomialHeap BHInsert(CBinomialHeap &h1, Node *n) {
        CBinomialHeap h2(n);
        CBinomialHeap C = BHMerge(h1, h2);
        return C;
    }

    static Node *BHMergeTree(Node *n1, Node *n2) {
        n1->sibling = nullptr;
        n2->sibling = nullptr;
        if (n1->val->getPopularity() > n2->val->getPopularity()) {
            n2->parent = n1;
            n2->sibling = n1->child;
            n1->child = n2;
            n1->degree++;
            return n1;
        } else if (n1->val->getPopularity() == n2->val->getPopularity()) {
            if (n1->val->getMTime() >= n2->val->getMTime()) {
                n2->parent = n1;
                n2->sibling = n1->child;
                n1->child = n2;
                n1->degree++;
                return n1;
            } else {
                n1->parent = n2;
                n1->sibling = n2->child;
                n2->child = n1;
                n2->degree++;
                return n2;
            }
        } else {
            n1->parent = n2;
            n1->sibling = n2->child;
            n2->child = n1;
            n2->degree++;
            return n2;
        }
    }

    static CBinomialHeap BHMerge(CBinomialHeap &A, CBinomialHeap &B) {
        CBinomialHeap C{};
        Node *CRoot = nullptr;
        Node *res = nullptr;
        Node *adds[3];
        Node *carry = nullptr;
        int notEmpty = 2;
        int currDeg = 0;
        Node *ARoot = A.root;
        Node *BRoot = B.root;
        while (notEmpty >= 2) {
            notEmpty = 0;
            int count = 0;
            if (ARoot) {
                notEmpty++;
                Node *a = ARoot;
                if (a->degree == currDeg) {
                    adds[count] = a;
                    count++;
                    ARoot = ARoot->sibling;
                }
            }
            if (BRoot) {
                notEmpty++;
                Node *b = BRoot;
                if (b->degree == currDeg) {
                    adds[count] = b;
                    count++;
                    BRoot = BRoot->sibling;
                }
            }
            if (carry) {
                notEmpty++;
                adds[count] = carry;
                count++;
                carry = nullptr;
            }
            if (count == 3) {
                if (!CRoot) {
                    CRoot = adds[2];
                    res = adds[2];
                } else {
                    CRoot->sibling = adds[2];
                    CRoot = CRoot->sibling;
                }
                carry = BHMergeTree(adds[0], adds[1]);
            } else if (count == 2) {
                carry = BHMergeTree(adds[0], adds[1]);
            } else if (count == 1) {
                if (!CRoot) {
                    CRoot = adds[0];
                    res = adds[0];
                } else {
                    CRoot->sibling = adds[0];
                    CRoot = CRoot->sibling;
                }
            }
            currDeg++;
        }
        if (!CRoot) {
            if (ARoot) {
                CRoot = ARoot;
                res = ARoot;
            }
        } else {
            if (ARoot) {
                CRoot->sibling = ARoot;
                CRoot = CRoot->sibling;
            }
        }
        if (!CRoot) {
            if (BRoot) {
                CRoot = BRoot;
                res = BRoot;
            }
        } else {
            if (BRoot) {
                CRoot->sibling = BRoot;
                CRoot = CRoot->sibling;
            }
        }

        if (A.root && B.max) {
            if (A.max->val->getPopularity() > B.max->val->getPopularity())
                C.max = A.max;
            else if (A.max->val->getPopularity() == B.max->val->getPopularity()) {
                if (A.max->val->getMTime() >= B.max->val->getMTime())
                    C.max = A.max;
                else
                    C.max = B.max;
            } else
                C.max = B.max;
        } else if (A.root) {
            C.max = A.max;
        } else if (B.root)
            C.max = B.max;

        if (res)
            C.root = res;
        return C;
    }

    static Node *BHReverseList(Node *n) {
        Node *tmp = n;
        Node *tmpNext;
        Node *newRoot = nullptr;

        while (tmp) {
            tmpNext = tmp->sibling;
            tmp->sibling = newRoot;
            newRoot = tmp;
            tmp = tmpNext;
        }
        return newRoot;
    }

    static Node *BHFindMax(CBinomialHeap &h) {
        Node *max = h.root;
        Node *tmp = h.root;
        while (tmp) {
            if (tmp->val->getPopularity() > max->val->getPopularity())
                max = tmp;
            else if (tmp->val->getPopularity() == max->val->getPopularity())
                if (tmp->val->getMTime() >= max->val->getMTime())
                    max = tmp;
            tmp = tmp->sibling;
        }
        return max;
    }

    static Node *BHGetMax(const CBinomialHeap &h) {
        return h.max;
    }

    static void BHExtractMax(CBinomialHeap &h) {
        if (!h.max || !h.root) throw std::runtime_error("Empty heap (min)");
        Node *tmp = h.root;
        Node *prev = nullptr;
        //connect binom trees without max
        while (tmp != h.max) {
            prev = tmp;
            tmp = tmp->sibling;
        }
        if (!prev)
            h.root = h.max->sibling;
        else
            prev->sibling = tmp->sibling;

        Node *next = h.max->child;
        tmp = next;

        while (tmp) { //set tu null
            tmp->parent = h.max->parent;
            tmp = tmp->sibling;
        }

        delete h.max;

        h.max = BHFindMax(h);
        next = BHReverseList(next);
        CBinomialHeap h2(next);
        h2.max = BHFindMax(h2);

        h = BHMerge(h, h2);
    }

    static void BHIncreaseKey(CBinomialHeap &h, Node *node, uint32_t val) {
        if (!node) return;
        node->val->setMPopularity(val);
        Node *parent = node->parent;

        while (parent && node->val->getPopularity() >= parent->val->getPopularity()) {
            std::swap(node->val, parent->val);
            std::swap(node->val->m_Node, parent->val->m_Node);
            node = parent;
            parent = parent->parent;
        }

        if (h.max->val->getPopularity() <= node->val->getPopularity())
            h.max = node;
    }

    static void BHDecreaseKey(CBinomialHeap &h, Node *node, uint32_t val) {
        if (!node) return;
        node->val->setMPopularity(val);
        CBinomialHeap heap = CBinomialHeap(node->child);
        Node *child = BHFindMax(heap);

        while (child && node->val->getPopularity() < child->val->getPopularity()) {
            std::swap(node->val, child->val);
            std::swap(node->val->m_Node, child->val->m_Node);
            node = child;
            child = child->child;
            heap = CBinomialHeap(child);
            child = BHFindMax(heap);
        }
/*
        if (h.max->val->getPopularity() <= node->val->getPopularity())
            h.max = node;
        */
        h.max = BHFindMax(h);

    }

    static void BHDelete(CBinomialHeap &h, Node *n) {
        BHIncreaseKey(h, n, UINT32_MAX);
        BHExtractMax(h);
    }

    static void printTree(Node *tmp) {
        Node *h = tmp;
        while (h) {
            std::cout << h->val->getPopularity() << " " << std::flush;
            printTree(h->child);
            h = h->sibling;
        }
    }

    // print function for binomial heap
    static void printHeap(CBinomialHeap &h) {
        Node *tmp = h.root;
        while (tmp) {
            std::cout << tmp->val->getPopularity() << " " << std::flush;
            printTree(tmp->child);
            tmp = tmp->sibling;
        }
        std::cout << std::endl;
    }

    static bool isEmpty(CBinomialHeap &h) {
        return h.root == nullptr;
    }

    Node *getRoot() const {
        return root;
    }
};

class CParty {
public:
    explicit CParty(uint32_t id) : m_id(id) {}

    void insert(Node *pol) {
        //std::cout << pol->val->getMName() << std::endl;
        m_Politicians = CBinomialHeap::BHInsert(m_Politicians, pol);
    }

    void deregister(CPolitician *p) {
        CBinomialHeap::BHDelete(m_Politicians, p->getMNode());
    }

    CPolitician *getLeader() {
        return CBinomialHeap::BHGetMax(m_Politicians)->val;
    }

    void changePopularity(CPolitician *p, uint32_t pop) {
        if (pop >= p->getPopularity())
            CBinomialHeap::BHIncreaseKey(m_Politicians, p->getMNode(), pop);
        else
            CBinomialHeap::BHDecreaseKey(m_Politicians, p->getMNode(), pop);

    }

    bool isEmpty() {
        return CBinomialHeap::isEmpty(m_Politicians);
    }

    void print() {
        CBinomialHeap::printHeap(m_Politicians);
    }

    void changeTree (Node* n, uint32_t id) {
        while(n) {
            n->val->setMPartyId(id);
            changeTree(n->child, id);
            n = n->sibling;
        }
    }

    void changeParty(CBinomialHeap& heap, uint32_t id) {
        Node* tmp = heap.getRoot();
        while (tmp) {
            tmp->val->setMPartyId(id);
            changeTree(tmp, id);
            tmp = tmp->sibling;
        }
    }

    void mergeParties(CParty *pParty) {
        changeParty(pParty->m_Politicians, CBinomialHeap::BHGetMax(m_Politicians)->val->getMPartyId());
        m_Politicians = CBinomialHeap::BHMerge(m_Politicians, pParty->m_Politicians);
    }

private:
    uint32_t m_id;
    CBinomialHeap m_Politicians;
};

class CPulitzer {
public:
    CPulitzer(size_t N, size_t P);

    ~CPulitzer();

    bool register_politician(uint32_t id_party, uint32_t id_politician, const std::string &name, uint32_t popularity,
                             uint8_t gender);

    bool politician_name(uint32_t id_politician, std::string &name) const;

    bool politician_gender(uint32_t id_politician, uint8_t &gender) const;

    bool politician_popularity(uint32_t id_politician, uint32_t &popularity) const;

    bool deregister_politician(uint32_t id_politician);

    bool party_leader(uint32_t id_party, uint32_t &id_leader) const;

    bool change_popularity(uint32_t id_politician, uint32_t popularity);

    bool sack_leader(uint32_t id_party);

    bool merge_parties(uint32_t dest_party, uint32_t src_party);

    bool create_coalition(uint32_t id_party1, uint32_t id_party2);

    bool leave_coalition(uint32_t id_party);

    bool coalition_leader(uint32_t id_party, uint32_t &id_leader) const;

    bool scandal_occured(uint32_t id_party);

    void print();

private:
    size_t m_MaxNumberOfParties;
    size_t m_MaxNumberOfPoliticians;
    uint64_t m_time{};
    CPolitician **m_Politicians; //pointers to politicians in BH
    CParty **m_Parties;
};

CPulitzer::CPulitzer(size_t N, size_t P) : m_MaxNumberOfParties(N), m_MaxNumberOfPoliticians(P) {
    if (N < 1 || P > 1000000)
        throw std::runtime_error("Invalid parameters");
    m_Politicians = new CPolitician *[m_MaxNumberOfPoliticians]();
    m_Parties = new CParty *[m_MaxNumberOfPoliticians]();
}

CPulitzer::~CPulitzer() {
    for (size_t i = 0; i < m_MaxNumberOfPoliticians; i++) {
        if (m_Politicians[i])
            delete m_Politicians[i]->m_Node;
    }
    for (size_t i = 0; i < m_MaxNumberOfParties; i++) {
        if (m_Parties[i])
            delete m_Parties[i];
    }
    delete[] m_Politicians;
    delete[] m_Parties;
}

bool
CPulitzer::register_politician(uint32_t id_party, uint32_t id_politician, const std::string &name, uint32_t popularity,
                               uint8_t gender) {
    if (id_politician >= m_MaxNumberOfPoliticians || id_party >= m_MaxNumberOfParties || m_Politicians[id_politician])
        return false;
    auto p = new CPolitician(name, popularity, gender, id_party, id_politician, m_time);
    m_time++;
    m_Politicians[id_politician] = p;
    p->setMNode(new Node(p));
    if (!m_Parties[id_party])
        m_Parties[id_party] = new CParty(id_party);
    m_Parties[id_party]->insert(m_Politicians[id_politician]->getMNode());
    return true;
}

bool CPulitzer::deregister_politician(uint32_t id_politician) {
    if (id_politician >= m_MaxNumberOfPoliticians || !m_Politicians[id_politician])
        return false;
    /*
    for (size_t i = 0; i < m_MaxNumberOfPoliticians; i++) {
        if (m_Politicians[i])
            std::cout << i << " " << m_Politicians[i]->getMId() << std::endl;
    }
    */
    uint32_t id_party = m_Politicians[id_politician]->getMPartyId();
    m_Parties[id_party]->deregister(m_Politicians[id_politician]);
    m_Politicians[id_politician] = nullptr;
    if (m_Parties[id_party]->isEmpty()) {
        delete m_Parties[id_party];
        m_Parties[id_party] = nullptr;
    }
    return true;
}
//change party id when changing party
bool CPulitzer::party_leader(uint32_t id_party, uint32_t &id_leader) const {
    if (id_party >= m_MaxNumberOfParties || !m_Parties[id_party])
        return false;
    CPolitician *leader = m_Parties[id_party]->getLeader();
    id_leader = leader->getMId();
    return true;
}

bool CPulitzer::change_popularity(uint32_t id_politician, uint32_t popularity) {
    if (id_politician >= m_MaxNumberOfPoliticians || !m_Politicians[id_politician])
        return false;
    m_Politicians[id_politician]->setMTime(m_time);
    m_time++;
    m_Parties[m_Politicians[id_politician]->getMPartyId()]->changePopularity(m_Politicians[id_politician], popularity);
    return true;
}

bool CPulitzer::sack_leader(uint32_t id_party) {
    if (id_party >= m_MaxNumberOfParties || !m_Parties[id_party])
        return false;
    return deregister_politician(m_Parties[id_party]->getLeader()->getMId());
}

bool CPulitzer::politician_popularity(uint32_t id_politician, uint32_t &popularity) const {
    if (id_politician >= m_MaxNumberOfPoliticians || !m_Politicians[id_politician])
        return false;
    popularity = m_Politicians[id_politician]->getPopularity();
    return true;
}

bool CPulitzer::politician_name(uint32_t id_politician, std::string &name) const {
    if (id_politician >= m_MaxNumberOfPoliticians || !m_Politicians[id_politician])
        return false;
    name = m_Politicians[id_politician]->getMName();
    return true;
}

bool CPulitzer::politician_gender(uint32_t id_politician, uint8_t &gender) const {
    if (id_politician >= m_MaxNumberOfPoliticians || !m_Politicians[id_politician])
        return false;
    gender = m_Politicians[id_politician]->getMGender();
    return true;
}

bool CPulitzer::scandal_occured(uint32_t id_party) {
    return sack_leader(id_party);
}

bool CPulitzer::leave_coalition(uint32_t id_party) {
    return false;
}

bool CPulitzer::coalition_leader(uint32_t id_party, uint32_t &id_leader) const {
    return party_leader(id_party, id_leader);
}

bool CPulitzer::create_coalition(uint32_t id_party1, uint32_t id_party2) {
    return false;
}

bool CPulitzer::merge_parties(uint32_t dest_party, uint32_t src_party) {
    if (!m_Parties[dest_party] || !m_Parties[src_party])
        return false;
    if (src_party == dest_party)
        return true;
    m_Parties[dest_party]->mergeParties(m_Parties[src_party]);
    delete m_Parties[src_party];
    m_Parties[src_party] = nullptr;
    return true;
}

void CPulitzer::print() {
    for (size_t i = 0; i < m_MaxNumberOfParties; i++) {
        if (m_Parties[i])
            m_Parties[i]->print();
    }
}