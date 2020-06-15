std::vector<int> getBlocks(std::vector<int> array) {
    std::vector<int> blocks;
    auto arrayIt = array.begin();
    while (arrayIt != array.end()) {
        while (*arrayIt == 0)
            arrayIt++;
        int size = 0;
        if (*arrayIt == 1) {
            while (*arrayIt == 1) {
                size++;
                arrayIt++;
            }
            blocks.emplace_back(size);
        }
    }
    return blocks;
}

double RecursiveMinMatching(std::vector<int> blocksA, std::vector<int> blocksB,
                            int i, int j, std::vector<std::pair<int,int>>* curMatching) {
    double peso = 0;
    if (i == 0) {
        peso = blocksA[0];
        int den = 0;
        for (int n = 0; n <= j; ++n) {
            den += blocksB[n];
            curMatching->emplace_back(1, n + 1);
        }
        peso /= den;
        return peso;
    }
    if (j == 0) {
        for (int n = 0; n <= i; ++n) {
            peso += blocksA[n];
            curMatching->emplace_back(n + 1, 1);
        }
        peso /= blocksB[0];
        return peso;
    }
    double curPeso = 100000;
    for (int x = 0; x <= i-1; ++x) { //Agrupar
        std::vector<std::pair<int,int>> newMatching;
        double agrupar = RecursiveMinMatching(blocksA, blocksB, x,j-1, &newMatching);
        double num = 0;
        for (int n = x+1; n <= i; ++n) {
            num += blocksA[n];
            newMatching.emplace_back(n+1, j+1);
        }
        agrupar += num/blocksB[j];
        if (agrupar < curPeso) {
            curPeso = agrupar;
            *curMatching = newMatching;
        }
    }
    for (int y = 0; y <= j-1; ++y) { //Dividir
        std::vector<std::pair<int,int>> newMatching;
        double dividir = RecursiveMinMatching(blocksA, blocksB, i-1, y, &newMatching);
        double den = 0;
        for (int n = y+1; n <= j; ++n) {
            den += blocksB[n];
            newMatching.emplace_back(i+1, n+1);
        }
        dividir += blocksA[i] / den;
        if (dividir < curPeso) {
            curPeso = dividir;
            *curMatching = newMatching;
        }
    }
    return curPeso;
}

int main() {
    std::vector<int> A = {0,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,1,0,1,0};
    std::vector<int> B = {0,0,1,1,0,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0};
    std::vector<std::pair<int,int>> matching;
    std::vector<int> blocksA = getBlocks(A);
    std::vector<int> blocksB = getBlocks(B);
    int m = blocksA.size();
    int n = blocksB.size();
    double peso = RecursiveMinMatching(blocksA, blocksB, m-1, n-1, &matching);
    for (auto pair : matching) {
        std::cout << "(" << pair.first << "," << pair.second << ")\n";
    }
    std::cout << "Peso: " << peso << std::endl;
}
