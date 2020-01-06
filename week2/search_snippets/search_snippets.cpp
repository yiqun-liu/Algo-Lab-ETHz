#include <cstdio>
#include <vector>
#include <queue> // for minheap

const int MAX_POS = 1 << 30;

class Word
{
    public:
        Word(int idx = 0, int pos = 0)
        {
            this->idx = idx;
            this->pos = pos;
        }
        bool operator<(const Word& w) const
        {
            return w.pos < this->pos; // actually the greater
        }
        int idx;
        int pos;
};

void testcase()
{
    int n;
    scanf("%d", &n);

    std::vector<int> m(n);
    std::vector<std::vector<int> > p(n);
    int length = 0;
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &m[i]);
        p[i].resize(m[i]);
        length += m[i];
    }
    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m[i]; j++)
            scanf("%d", &p[i][j]);
    
    // Merge sorted subsequences
    std::vector<Word> words(length);
    std::priority_queue<Word> minHeap;
    for (int i = 0; i < n; i++) // init min heap
        minHeap.push(Word(i, p[i][0]));
        
    std::vector<int> mergeIdx(n, 1); // index set to 1 as we have pushed them already
    for (int i = 0; i < length; i++)
    {
        words[i] = minHeap.top();
        minHeap.pop();
        
        int &wordIdx = words[i].idx;
        if (mergeIdx[ wordIdx ] < m[ wordIdx ])
        {
            minHeap.push(Word( wordIdx, p[wordIdx][mergeIdx[wordIdx]] ));
            mergeIdx[wordIdx]++;
        }
    }

    // Now sliding window
    int l = 0, r = -1, count = 0;
    int minSnippetLength = MAX_POS;
    std::vector<int> wordCount(n, 0);
    while (r < length)
    {
        // Find a feasible interval
        while (r + 1 < length && count < n)
        {
            ++r;
            if (wordCount[ words[r].idx ] == 0)
                count++;
            wordCount[ words[r].idx ]++;
        }

        if (count < n)
            break;
        
        // Compress
        while( l < r && wordCount[ words[l].idx ] > 1)
            wordCount[ words[l++].idx ]--;

        int snippetLength = words[r].pos - words[l].pos + 1;
        if (snippetLength < minSnippetLength)
            minSnippetLength = snippetLength;
        
        // Move forward
        count--;
        wordCount[ words[l++].idx ]--;
    }
    
    printf("%d\n", minSnippetLength);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}