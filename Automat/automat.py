class automat:
    def __init__(self, Alf, S, s, F):
        self.alfabet = Alf
        self.stari = S
        self.initial = s
        self.finale = F
        self.legat = [[[] for x in range(len(S))] for y in range(len(S))]
        self.dict = {}      # stare -> index
        self.dictR = {}     # index -> stare
        self.valid = False

    def validare(self, stare, cuvant, k, n):
        if k > n:
            self.valid = True
        if k == n:
            for i in range(len(self.stari)):
                if cuvant[k] in self.legat[self.dict[stare]][i] and self.dictR[i] in self.finale:
                    self.valid = True
        else:
            for i in range(len(self.stari)):
                if cuvant[k] in self.legat[self.dict[stare]][i]:
                    self.validare(self.dictR[i], cuvant, k+1, n)

    def creeaza_legaturi(self, legaturi):
        k = 0
        for legatura in legaturi:
            if legatura[0] not in self.dict:
                self.dict[legatura[0]] = k
                self.dictR[k] = legatura[0]
                k += 1
            if legatura[1] not in self.dict:
                self.dict[legatura[1]] = k
                self.dictR[k] = legatura[1]
                k += 1
            self.legat[self.dict[legatura[0]]][self.dict[legatura[1]]].append(legatura[2])


def citire():
    f = open("date.in", 'r')
    alfabet = f.readline().split()
    stari = f.readline().split()
    initial = f.readline().strip()
    finale = f.readline().split()
    legaturi = []
    for linie in f.readlines():
        legaturi.append(tuple([x for x in linie.split()]))
    A = automat(alfabet, stari, initial, finale)
    A.creeaza_legaturi(legaturi)
    f.close()
    return A


A = citire()
inp = "abaccc"
A.valid = False
A.validare(A.initial, inp, 0, len(inp)-1)
print(A.valid)



