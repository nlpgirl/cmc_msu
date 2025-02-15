import task5

def write_to_file(filename, data, language):
    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"Частотный словарь для {language} текста:\n")
        f.write("=" * 40 + "\n")
        for word, freq in data.items():
            f.write(f"Слово: '{word}', Частота: {freq}\n")
        f.write("\n")
        f.write("Общее количество уникальных слов: {}\n".format(len(data)))
        f.write("=" * 40 + "\n")

# Запись частотных словарей в файлы с пояснениями
write_to_file('frequency_russian.txt', task5.freq_russian, "русского")
write_to_file('frequency_turkish.txt', task5.freq_turkish, "турецкого")
