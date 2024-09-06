from scipy.stats import spearmanr
from gensim.models import KeyedVectors

model = KeyedVectors.load_word2vec_format('wiki-news-300d-1M.vec')

def load_goldstandard(file_name):
    goldstandard = {}
    with open(file_name, 'r') as file:
        for line in file:
            word1, word2, score = line.strip().split('\t')
            goldstandard[(word1, word2)] = float(score)
    return goldstandard

def calc_similarity(word1, word2):
    similarity = 0
    if word1 in model.index_to_key and word2 in model.index_to_key:
            similarity = model.similarity(word1, word2)
    return similarity

def spearman(values1, values2):
    spearman_correlation, _ = spearmanr(values1, values2)
    return spearman_correlation

def get_values(similarities):
    return [value for (word1, word2), value in similarities.items()]

def get_results(goldstandard_file_name):
    similarities = {}
    goldstandard = load_goldstandard(goldstandard_file_name)
    for (word1, word2), score in goldstandard.items():
        similarities[(word1, word2)] = calc_similarity(word1, word2)
    goldstandard_values = list(goldstandard.values())
    print(spearman(get_values(similarities), goldstandard_values))
    

print("Spearman corellation for wordsim_similarity_goldstandard.txt:")
get_results(r'C:\Users\aygul\Desktop\NLP\wordsim353_sim_rel\wordsim_similarity_goldstandard.txt')
print("Spearman corellation for wordsim_relatedness_goldstandard.txt:")
get_results(r'C:\Users\aygul\Desktop\NLP\wordsim353_sim_rel\wordsim_relatedness_goldstandard.txt')