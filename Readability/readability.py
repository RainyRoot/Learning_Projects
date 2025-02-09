from cs50 import get_string

user_input = get_string("Text: ")
i = 0
k = 0
j = 0
letter_count = 0
word_count = 1
sentence_count = 0
end_of_sentence = [33, 46, 63]

text = user_input.lower()

# while it's not end of the text
while i < len(text):
    # check if it's a letter
    if text[i].isalpha():
        letter_count += 1
    # otherwise skip
    i += 1

# while not at end of text
while k < len(text):
    # check for space (ASCII)
    if text[k] == ' ':
        word_count += 1
    # if no space, go to next char
    k += 1

# check if ! . ? (end_of_sentence) -> conclude end of sentence
while j < len(text):
    if ord(text[j]) in end_of_sentence:
        sentence_count += 1
    j += 1

# Avg number of letters per 100 words
l = (letter_count / word_count) * 100
# Avg number of sentences per 100 words
s = (sentence_count / word_count) * 100

# Coleman-Liau index
grade = round(0.0588 * l - 0.296 * s - 15.8)

# check the grade value and print accordingly
if grade <= 16 and grade >= 1:
    print(f"Grade {grade}")
elif grade < 1:
    print("Before Grade 1")
else:
    print("Grade 16+")
