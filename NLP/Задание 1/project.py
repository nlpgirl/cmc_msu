import nltk
from nltk.corpus import wordnet as wn
from nltk.corpus import genesis
genesis_ic = wn.ic(genesis, False, 0.0)
from scipy.stats import spearmanr

# загрузка данных из файлов
def load_goldstandard_sim():
    goldstandard = {}
    with open('wordsim353_sim_rel/wordsim_similarity_goldstandard.txt', 'r') as file:
        for line in file:
            word1, word2, score = line.strip().split('\t')
            goldstandard[(word1, word2)] = float(score)
    print("sorted goldstandart_sim:", sorted(goldstandard.items(), key=lambda x: x[1], reverse=True))
    return goldstandard

def load_goldstandard_rel():
    goldstandard = {}
    with open('wordsim353_sim_rel/wordsim_relatedness_goldstandard.txt', 'r') as file:
        for line in file:
            word1, word2, score = line.strip().split('\t')
            goldstandard[(word1, word2)] = float(score)
    return goldstandard


# функция для вычисления близости слов методом LCH
def lch_similarity(word1, word2):
    synsets1 = wn.synsets(word1)
    synsets2 = wn.synsets(word2)

    similarities = []

    for synset1 in synsets1:
        for synset2 in synsets2:
            if synset1.pos() == synset2.pos():
                lch_similarity = synset1.lch_similarity(synset2)
                similarities.append(lch_similarity)
    if len(similarities) == 0:
        return 0
    return max(similarities)


# функция для вычисления близости слов методом WUP
def wup_similarity(word1, word2):
    synsets1 = wn.synsets(word1)
    synsets2 = wn.synsets(word2)

    similarities = []

    for synset1 in synsets1:
        for synset2 in synsets2:
            if synset1.pos() == synset2.pos():
                wup_similarity = synset1.wup_similarity(synset2)
                similarities.append(wup_similarity)

    if len(similarities) == 0:
        return 0
    return max(similarities)


# функция для вычисления близости слов методом JCN
def jcn_similarity(word1, word2):
    synsets1 = wn.synsets(word1)
    synsets2 = wn.synsets(word2)
    similarities = []

    for synset1 in synsets1:
        for synset2 in synsets2:
            if synset1.pos() == synset2.pos():
                jcn_similarity = synset1.jcn_similarity(synset2, genesis_ic)
                similarities.append(jcn_similarity)

    if len(similarities) == 0:
        return 0
    return max(similarities)


# функция для ранжирования по мере снижения близости
def rank_similarity(similarities):
    ranked_similarities = sorted(similarities.items(), key=lambda x: x[1], reverse=True)
    return ranked_similarities

# функция для вычисления коэффициента корреляции Спирмена
def spearman(values1, values2):
    spearman_correlation, _ = spearmanr(values1, values2)
    return spearman_correlation

def get_values(similarities):
    return [value for (word1, word2), value in similarities.items()]

def print_res(goldstandard):
    lch_similarities = {}
    wup_similarities = {}
    jcn_similarities = {}

    for (word1, word2), score in goldstandard.items():
        lch_similarities[(word1, word2)] = lch_similarity(word1, word2)
        wup_similarities[(word1, word2)] = wup_similarity(word1, word2)
        jcn_similarities[(word1, word2)] = jcn_similarity(word1, word2)

    ranked_lch = rank_similarity(lch_similarities)
    ranked_wup = rank_similarity(wup_similarities)
    ranked_jcn = rank_similarity(jcn_similarities)

    # вывод ранжированных списков после применения методов LCH, WUP, JCN
    print("Ranked lists:")
    print("LCH:", ranked_lch)
    print("WUP:", ranked_wup)
    print("JCN:", ranked_jcn)

    goldstandard_values = list(goldstandard.values())

    # вывод мер Спирмена
    print("Spearman corellations:")
    print("LCH:", spearman(get_values(lch_similarities), goldstandard_values))
    print("WUP:", spearman(get_values(wup_similarities), goldstandard_values))
    print("JCN:", spearman(get_values(jcn_similarities), goldstandard_values))


# основная программа
nltk.download('wordnet')
nltk.download('genesis')

# similarity
print("Results for wordsim_similarity_goldstandart.txt:")
print_res(load_goldstandard_sim())

# relatedness
print("Results for wordsim_relatedness_goldstandart.txt:")
print_res(load_goldstandard_rel())
