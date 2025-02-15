# Условие задачи:
# Написать программу для сравнения словарного состава параллельных текстов
# на двух языках (например, русском и турецком). Программа должна:
#
# 1. Составить частотные словари, определить объем уникальных лемм
# при помощи collections.Counter, set().
#
# 2. Оценить число уникальных лемм по частям речи и самые частотные слова
# при помощи библиотеки pymorphy2 (для русского), zeyrek (морфологический анализатор для турецкого), Counter.
#
# 3. Рассчитать коэффициент лексического богатства, поделив количество элементов в списке лемм
# на количество элементов в множестве уникальных лемм (len(set(lemmas)) / len(lemmas)).
#
# 4. Визуализировать результаты при помощи matplotlib, pandas

import pymorphy3  # морфологический анализатор для русского языка
import zeyrek  # морфологический анализатор для турецкого языка
import re # библиотека для проверки формата слова
from collections import Counter  # функция для подсчета количества лемм
import matplotlib.pyplot as plt  # библиотека для выведения диаграмм
import pandas as pd  # библиотека для подсчета частот
from nltk.tokenize import word_tokenize  # функция токенизации

# Инициализация морфологического анализатора для русского и турецкого языков
morph_russian = pymorphy3.MorphAnalyzer()
analyzer_turkish = zeyrek.MorphAnalyzer()

def is_russian_word(word):
    pattern = r'^[а-яА-ЯёЁ\-]+$'  # регулярное выражение для русских букв и дефиса
    return bool(re.match(pattern, word))


def is_turkish_word(word):
    pattern = r'^[a-zA-ZğĞüÜşŞıİöÖçÇ\-]+$'  # регулярное выражение для турецких букв и дефиса
    return bool(re.match(pattern, word))

# Функция для загрузки текстов
def load_text(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        return file.read()

# Функция для предобработки текста (очистка и токенизация)
def preprocess_text_rus(text):
    tokens = word_tokenize(text.lower(), language="russian")
    tokens = [word for word in tokens if is_russian_word(word)]
    return tokens

def preprocess_text_tr(text):
    tokens = word_tokenize(text.lower(), language="turkish")
    tokens = [word for word in tokens if is_turkish_word(word)]
    return tokens

# Лемматизация и определение частей речи для русского текста
def lemmatize_and_tag_russian(tokens):
    lemmas_and_pos = []
    for word in tokens:
        parsed_word = morph_russian.parse(word)[0]
        lemmas_and_pos.append((parsed_word.normal_form, parsed_word.tag.POS))
    return lemmas_and_pos

# Лемматизация и определение частей речи для турецкого текста
def lemmatize_and_tag_turkish(tokens):
    lemmas_and_pos = []
    for word in tokens:
        analysis = analyzer_turkish.analyze(word)
        if analysis:
            lemma = analysis[0][0].lemma
            pos = analysis[0][0].pos
            lemmas_and_pos.append((lemma, pos))
        else:
            lemmas_and_pos.append((word, "UNK"))  # Если часть речи неизвестна
    return lemmas_and_pos

# Частотный словарь
def get_frequency_dict(lemmas):
    return Counter(lemmas)

# Подсчет коэффициента лексического богатства
def lexical_richness(lemmas):
    return len(set(lemmas)) / len(lemmas)

# Частотный словарь по частям речи
def get_pos_frequency_dict(lemmas_and_pos):
    pos_freq = Counter([pos for _, pos in lemmas_and_pos])
    return pos_freq

# Частотный словарь для каждой части речи
def get_freq_by_pos(lemmas_and_pos, target_pos):
    words_by_pos = [lemma for lemma, pos in lemmas_and_pos if pos == target_pos]
    return Counter(words_by_pos)

# Подсчет уникальных лемм для каждой части речи
def get_unique_lemmas_by_pos(lemmas_and_pos, target_pos):
    unique_lemmas = {lemma for lemma, pos in lemmas_and_pos if pos == target_pos}
    return len(unique_lemmas)

# Функция для записи вывода в файл
def write_to_file(filename, data):
    with open(filename, 'w', encoding='utf-8') as file:
        file.write(data)

# Визуализация всех графиков в одной фигуре
def visualize_all(freq_russian, freq_turkish, pos_freq_russian, pos_freq_turkish,
                  freq_nouns_russian, freq_verbs_russian, freq_adjectives_russian,
                  freq_nouns_turkish, freq_verbs_turkish, freq_adjectives_turkish):
    fig, axes = plt.subplots(3, 4, figsize=(20, 15))  # Создаем сетку 3x4 для 12 графиков

    # Графики для частот русских и турецких слов
    df_russian = pd.DataFrame(freq_russian.items(), columns=['Word', 'Frequency']).sort_values(by='Frequency',
                                                                                               ascending=False).head(10)
    df_russian.plot.barh(x='Word', y='Frequency', ax=axes[0, 0], title='Top 10 Frequent Words in Russian Text')

    df_turkish = pd.DataFrame(freq_turkish.items(), columns=['Word', 'Frequency']).sort_values(by='Frequency',
                                                                                               ascending=False).head(10)
    df_turkish.plot.barh(x='Word', y='Frequency', ax=axes[0, 1], title='Top 10 Frequent Words in Turkish Text')

    # Графики частот частей речи (POS) для русского и турецкого текста
    df_pos_russian = pd.DataFrame(pos_freq_russian.items(), columns=['POS', 'Frequency'])
    df_pos_russian.plot.barh(x='POS', y='Frequency', ax=axes[1, 0], title='POS Frequency in Russian Text')

    df_pos_turkish = pd.DataFrame(pos_freq_turkish.items(), columns=['POS', 'Frequency'])
    df_pos_turkish.plot.barh(x='POS', y='Frequency', ax=axes[1, 1], title='POS Frequency in Turkish Text')

    # Частотные слова по существительным, глаголам и прилагательным для русского текста
    df_nouns_russian = pd.DataFrame(freq_nouns_russian.items(), columns=['Noun (Word)', 'Frequency']).sort_values(
        by='Frequency', ascending=False).head(10)
    df_nouns_russian.plot.barh(x='Noun (Word)', y='Frequency', ax=axes[2, 0], title='Top 10 Nouns in Russian Text')

    df_verbs_russian = pd.DataFrame(freq_verbs_russian.items(), columns=['Verb (Word)', 'Frequency']).sort_values(
        by='Frequency', ascending=False).head(10)
    df_verbs_russian.plot.barh(x='Verb (Word)', y='Frequency', ax=axes[2, 1], title='Top 10 Verbs in Russian Text')

    df_adjectives_russian = pd.DataFrame(freq_adjectives_russian.items(),
                                         columns=['Adjective (Word)', 'Frequency']).sort_values(by='Frequency',
                                                                                                ascending=False).head(
        10)
    df_adjectives_russian.plot.barh(x='Adjective (Word)', y='Frequency', ax=axes[2, 2],
                                   title='Top 10 Adjectives in Russian Text')

    # Частотные слова по существительным, глаголам и прилагательным для турецкого текста
    df_nouns_turkish = pd.DataFrame(freq_nouns_turkish.items(), columns=['Noun (Word)', 'Frequency']).sort_values(
        by='Frequency', ascending=False).head(10)
    df_nouns_turkish.plot.barh(x='Noun (Word)', y='Frequency', ax=axes[2, 3], title='Top 10 Nouns in Turkish Text')

    df_verbs_turkish = pd.DataFrame(freq_verbs_turkish.items(), columns=['Verb (Word)', 'Frequency']).sort_values(
        by='Frequency', ascending=False).head(10)
    df_verbs_turkish.plot.barh(x='Verb (Word)', y='Frequency', ax=axes[1, 2], title='Top 10 Verbs in Turkish Text')

    df_adjectives_turkish = pd.DataFrame(freq_adjectives_turkish.items(),
                                         columns=['Adjective (Word)', 'Frequency']).sort_values(by='Frequency',
                                                                                                ascending=False).head(
        10)
    df_adjectives_turkish.plot.barh(x='Adjective (Word)', y='Frequency', ax=axes[1, 3],
                                   title='Top 10 Adjectives in Turkish Text')

    plt.subplots_adjust(left=0.08, bottom=0.072, right=0.962, top=0.97, wspace=0.526, hspace=0.36)
    plt.show()




# Загрузка и обработка русского текста
text_russian = load_text(input("Введите названия файла для русского языка: "))
tokens_russian = preprocess_text_rus(text_russian)
lemmas_and_pos_russian = lemmatize_and_tag_russian(tokens_russian)
lemmas_russian = [lemma for lemma, _ in lemmas_and_pos_russian]
freq_russian = get_frequency_dict(lemmas_russian)

# Загрузка и обработка турецкого текста
text_turkish = load_text(input("Введите название файла для турецкого языка: "))
tokens_turkish = preprocess_text_tr(text_turkish)
lemmas_and_pos_turkish = lemmatize_and_tag_turkish(tokens_turkish)
lemmas_turkish = [lemma for lemma, _ in lemmas_and_pos_turkish]
freq_turkish = get_frequency_dict(lemmas_turkish)

# Создание итогового текста для записи в файл
output = f"""
Объем частотного словаря для русского текста (уникальные леммы): {len(set(lemmas_russian))}
Объем частотного словаря для турецкого текста (уникальные леммы): {len(set(lemmas_turkish))}

Частотный словарь русского текста: {freq_russian.most_common(10)}
Частотный словарь турецкого текста: {freq_turkish.most_common(10)}

Коэффициент лексического богатства для русского текста: {lexical_richness(lemmas_russian):.5f}
Коэффициент лексического богатства для турецкого текста: {lexical_richness(lemmas_turkish):.5f}

Уникальные существительные в русском тексте: {get_unique_lemmas_by_pos(lemmas_and_pos_russian, 'NOUN')}
Уникальные глаголы в русском тексте: {get_unique_lemmas_by_pos(lemmas_and_pos_russian, 'VERB')}
Уникальные прилагательные в русском тексте: {get_unique_lemmas_by_pos(lemmas_and_pos_russian, 'ADJF')}

Уникальные существительные в турецком тексте: {get_unique_lemmas_by_pos(lemmas_and_pos_turkish, 'Noun')}
Уникальные глаголы в турецком тексте: {get_unique_lemmas_by_pos(lemmas_and_pos_turkish, 'Verb')}
Уникальные прилагательные в турецком тексте: {get_unique_lemmas_by_pos(lemmas_and_pos_turkish, 'Adj')}
"""

# Запись вывода в файл
write_to_file("results.txt", output)

pos_freq_russian = get_pos_frequency_dict(lemmas_and_pos_russian)
pos_freq_turkish = get_pos_frequency_dict(lemmas_and_pos_turkish)

# Частотные слова по частям речи (существительные, глаголы, прилагательные)
freq_nouns_russian = get_freq_by_pos(lemmas_and_pos_russian, 'NOUN')
freq_verbs_russian = get_freq_by_pos(lemmas_and_pos_russian, 'VERB')
freq_adjectives_russian = get_freq_by_pos(lemmas_and_pos_russian, 'ADJF')

freq_nouns_turkish = get_freq_by_pos(lemmas_and_pos_turkish, 'Noun')
freq_verbs_turkish = get_freq_by_pos(lemmas_and_pos_turkish, 'Verb')
freq_adjectives_turkish = get_freq_by_pos(lemmas_and_pos_turkish, 'Adj')

# Визуализация
visualize_all(freq_russian, freq_turkish, pos_freq_russian, pos_freq_turkish,
              freq_nouns_russian, freq_verbs_russian, freq_adjectives_russian,
              freq_nouns_turkish, freq_verbs_turkish, freq_adjectives_turkish)



