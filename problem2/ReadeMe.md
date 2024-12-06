# Social Media Risk Post Priority Queue Testing

This project implements a priority queue based on a heap structure to handle risk posts on a social media platform. To evaluate the performance and memory usage of the implementation, we've created a `test.c` file to measure the time and space complexities of various operations. This `README` provides instructions on how to compile and run the tests, as well as how to interpret the results.

## File Structure

- `SocialMediaStorage.h`: Defines the `Post` and `Storage` structures and the function prototypes for creating and managing the queue (e.g., `create_queue`, `insert_post`, `retrieve_post`, `delete_post`, etc.).
- `poststorage.c`: Contains the implementation of the priority queue, including insertion, deletion, and retrieval operations.
- `test.c`: Contains the code for testing the queue operations, measuring the time complexity and space usage.

## Testing Features

In `test.c`, we implemented tests for the following operations:

1. **Time Testing**: Using `clock()` to measure the time taken by each operation (e.g., creating the queue, inserting posts, deleting posts, etc.).
2. **Space Testing**: Measuring the memory usage before and after key operations to estimate memory consumption.

### Test Operations

- **Queue Creation** (`create_queue`): Measures the time taken to create the queue.
- **Post Insertion** (`insert_post`): Measures the time taken to insert several posts into the queue.
- **Retrieve Top Priority Post** (`retrieve_post`): Measures the time taken to retrieve the highest-priority post.
- **Post Deletion** (`delete_post`): Measures the time taken to delete the highest-priority post and re-adjust the heap.
- **Capacity Check** (`check_capacity`): Measures the time taken to check the remaining capacity of the queue.
- **Delete All Posts**: Measures the total time taken to delete all posts from the queue.

## Usage Instructions

### 1. Compile the Code

Ensure you have a C compiler (e.g., `gcc`) installed. To compile the code, use the following command:

```bash
gcc -o test test.c poststorage.c -std=c99
```

### 2. Run the Tests

After compiling, run the tests with the following command:

```bash
./test
```

### 3. Output Explanation

The program will output the time taken by each operation and the memory usage of the queue. Here is an example output:

```bash
Time to create queue: 0.000020 seconds
Time to insert posts: 0.215040 seconds
Queue after inserting posts:
Post ID: 0, Priority: 92
Post ID: 1, Priority: 33
...

Time to retrieve top priority post: 0.000010 seconds
Top priority post:
Post ID: 0, Priority: 92

Time to delete top priority post: 0.000012 seconds
Queue after deleting top priority post:
Post ID: 1, Priority: 33
...

Remaining capacity in the queue: 999

Time to delete all posts: 0.308234 seconds
Queue after deleting all posts:

Memory usage before insertion: 4000 bytes

```

### 4. Modify Test Parameters

You can change the number of posts to insert in the test by modifying the `MAX_POSTS` constant. For example, change this line in `test.c`:

```bash
#define MAX_POSTS 1000  // Modify this value to change the number of posts
```

After modifying the value, recompile and run the tests to observe how performance scales with different data sizes.

## Notes

1. **Memory Usage**: The space testing in `test.c` estimates memory usage by comparing the memory before and after key operations. For more detailed memory analysis, you can use tools like `valgrind`.
2. **Randomness**: Since post priorities are randomly generated, the results may vary between test runs. To ensure consistent results, you can set a fixed random seed in the code:

```bash
srand(12345);  // Set a fixed seed for reproducible tests
```

### 5. Further Optimizations

- You can optimize memory allocation by using a more efficient memory management strategy or modifying the heap operations.
- If you have specific performance requirements or want to test with larger datasets, you can tweak the `MAX_POSTS`constant or adjust the operations accordingly.

## Conclusion

This test framework provides a simple way to evaluate the performance and memory usage of the priority queue implementation. You can adjust the parameters and operations to test different scenarios and optimize the code as needed. If you have any questions, feel free to ask other team members for clarification.