
# Wordle Solver using Trie

This project implements an efficient Wordle assistant to help narrow down possible target words based on feedback from your guesses. It uses a **Trie**-based approach and a filtering logic that leverages a concise description of the feedback received in each attempt.


## :jigsaw: Problem Statement

[Wordle](https://www.nytimes.com/games/wordle/index.html) is a word guessing game where:
- You must guess a hidden word of fixed length `L`.
- For each guess, you receive a **description** consisting of:
  - :green_square: **Green (g)**: Correct letter in the correct position.
  - :yellow_square: **Yellow (y)**: Correct letter in the wrong position.
  - :white_large_square: **Grey (x)**: Letter not in the word at all.

Your task is to efficiently filter a large dictionary of words (`n` total words) and return all valid candidates that match the accumulated description over multiple guesses.

## :hammer_and_wrench: How It Works

This solver uses:
- A **Trie** data structure to store and traverse the dictionary of words.
- A **description-based filter** (implemented via scoped letter sets and inclusion constraints).
- A **DFS-based traversal** of the Trie to efficiently generate the list of valid words.

### :blue_book: Terminology

| Term         | Description                                                                 |
|--------------|-----------------------------------------------------------------------------|
| `L`          | Length of each word. All words in the dictionary are of the same length.   |
| `n`          | Number of words in the dictionary.                                          |
| `description`| A mapping derived from feedback (Green, Yellow, Grey) for a given guess.    |

## :mag: Example

Given a guess: `table`  
And feedback: `xgxyx`  
This means:
- `t` is not in the word (grey),
- `a` is correct and in the correct position (green),
- `b` is in the word but at a different position (yellow),
- `l`, `e` are not in the word.

The description gets updated accordingly and helps filter the dictionary.

## Input Format
On each iteration, input your guess and the feedback in this format:

```bash
GUESS FEEDBACK
```

###### Example:

```bash
table xgxyx
```

The program will print all matching words that comply with the cumulative description so far.

## :bar_chart: Time and Space Complexity

### :brain: Space Complexity:

- Trie storage: O(n * L) for n words of length L.
- Scope matrix: O(26 * L) worst case, storing all possible letters per position.
- Include set: O(26) (constant space).

**Total**: O(n * L)

### :stopwatch: Time Complexity:
- Insertion of all words: O(n * L)
- Each guess filtering:
    - Worst-case DFS traversal: O(k), where k is the number of matching nodes in the Trie, bounded by filtered candidate words.
    - Filtering is pruned using scoped allowed characters and inclusion set.

**Efficient for reasonable dictionary sizes** (n ≈ 10⁴–10⁵).

## :brain: Features & Design Highlights

- Robust Feedback Handling: Handles multiple occurrences of letters gracefully using a per-letter feed_map.
- Efficient Filtering: Prunes non-matching branches early using scope constraints and inclusion requirements.
- Flexible Word Length (L): Works for any word length by changing a single parameter.
- Real-Time Iteration: Interactive loop allows continuous guessing and feedback.

## :godmode: Author

This tool was designed to explore one of the most efficient logics for solving Wordle using structured filtering and optimized Trie traversal.

## :page_with_curl: License

This project is released under the MIT License.
