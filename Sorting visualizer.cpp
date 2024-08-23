#include <SDL.h>
#include <iostream>
#include <limits>
#include <ctime>
#include <string>
using namespace std;

// Screen dimensions
const int SCREEN_WIDTH = 910;
const int SCREEN_HEIGHT = 750;

// Array size and rectangle size for visualization
const int ARRAY_SIZE = 130;
const int RECT_SIZE = 7;

// Array to hold the data and backup array
int dataArray[ARRAY_SIZE];
int backupArray[ARRAY_SIZE];

// SDL Window and Renderer pointers
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// Flag to check if sorting is complete
bool isSortingComplete = false;

// Function to initialize SDL
bool initializeSDL()
{
    bool isSuccess = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Couldn't initialize SDL. SDL_Error: " << SDL_GetError();
        isSuccess = false;
    }
    else
    {
        // Set the rendering scale quality hint
        if (!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")))
        {
            cout << "Warning: Linear Texture Filtering not enabled.\n";
        }

        // Create SDL Window
        window = SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            cout << "Couldn't create window. SDL_Error: " << SDL_GetError();
            isSuccess = false;
        }
        else
        {
            // Create SDL Renderer
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                cout << "Couldn't create renderer. SDL_Error: " << SDL_GetError();
                isSuccess = false;
            }
        }
    }

    return isSuccess;
}

// Function to clean up SDL resources
void cleanUpSDL()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

// Function to visualize the sorting process
void visualizeSorting(int highlightIndex1 = -1, int highlightIndex2 = -1, int highlightIndex3 = -1)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Set background color to black
    SDL_RenderClear(renderer);

    int index = 0;
    for (int i = 0; i <= SCREEN_WIDTH - RECT_SIZE; i += RECT_SIZE)
    {
        SDL_PumpEvents(); // Process SDL events

        SDL_Rect rect = { i, 0, RECT_SIZE, dataArray[index] }; // Define the rectangle for visualization
        if (isSortingComplete)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0); // Color for sorted elements
            SDL_RenderDrawRect(renderer, &rect);
        }
        else if (index == highlightIndex1 || index == highlightIndex3)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0); // Color for elements being compared
            SDL_RenderFillRect(renderer, &rect);
        }
        else if (index == highlightIndex2)
        {
            SDL_SetRenderDrawColor(renderer, 165, 105, 189, 0); // Color for the currently sorted element
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 170, 183, 184, 0); // Color for other elements
            SDL_RenderDrawRect(renderer, &rect);
        }
        index++;
    }
    SDL_RenderPresent(renderer); // Present the rendered image
}

// Function to perform heap sort
void heapSort(int* array, int size)
{
    // Build the heap
    for (int i = 1; i < size; i++)
    {
        int childIndex = i;
        int parentIndex = (childIndex - 1) / 2;

        while (childIndex > 0)
        {
            if (array[childIndex] > array[parentIndex])
            {
                // Swap values
                int temp = array[parentIndex];
                array[parentIndex] = array[childIndex];
                array[childIndex] = temp;
            }
            else
            {
                break;
            }

            visualizeSorting(parentIndex, childIndex); // Visualize sorting
            SDL_Delay(40); // Delay for visualization

            childIndex = parentIndex;
            parentIndex = (childIndex - 1) / 2;
        }
    }

    // Sort the heap
    for (int heapEnd = size - 1; heapEnd >= 0; heapEnd--)
    {
        // Swap root with the end of the heap
        int temp = array[0];
        array[0] = array[heapEnd];
        array[heapEnd] = temp;

        int parentIndex = 0;
        int leftChildIndex = 2 * parentIndex + 1;
        int rightChildIndex = 2 * parentIndex + 2;

        while (leftChildIndex < heapEnd)
        {
            int maxIndex = parentIndex;

            // Check left child
            if (array[leftChildIndex] > array[maxIndex])
            {
                maxIndex = leftChildIndex;
            }
            // Check right child
            if (rightChildIndex < heapEnd && array[rightChildIndex] > array[maxIndex])
            {
                maxIndex = rightChildIndex;
            }
            if (maxIndex == parentIndex)
            {
                break;
            }

            // Swap values
            int temp = array[parentIndex];
            array[parentIndex] = array[maxIndex];
            array[maxIndex] = temp;

            visualizeSorting(maxIndex, parentIndex, heapEnd); // Visualize sorting
            SDL_Delay(40); // Delay for visualization

            parentIndex = maxIndex;
            leftChildIndex = 2 * parentIndex + 1;
            rightChildIndex = 2 * parentIndex + 2;
        }
    }
}

// Function to partition the array for quicksort
int partitionArray(int array[], int startIndex, int endIndex)
{
    int smallerCount = 0;

    for (int i = (startIndex + 1); i <= endIndex; i++)
    {
        if (array[i] <= array[startIndex])
        {
            smallerCount++;
        }
    }
    int pivotIndex = startIndex + smallerCount;
    int temp = array[pivotIndex];
    array[pivotIndex] = array[startIndex];
    array[startIndex] = temp;
    visualizeSorting(pivotIndex, startIndex);

    int leftIndex = startIndex, rightIndex = endIndex;

    while (leftIndex < pivotIndex && rightIndex > pivotIndex)
    {
        if (array[leftIndex] <= array[pivotIndex])
        {
            leftIndex++;
        }
        else if (array[rightIndex] > array[pivotIndex])
        {
            rightIndex--;
        }
        else
        {
            int temp = array[rightIndex];
            array[rightIndex] = array[leftIndex];
            array[leftIndex] = temp;

            visualizeSorting(leftIndex, rightIndex);
            SDL_Delay(70); // Delay for visualization

            leftIndex++;
            rightIndex--;
        }
    }
    return pivotIndex;
}

// Function to perform quicksort
void quickSort(int array[], int startIndex, int endIndex)
{
    if (startIndex >= endIndex)
    {
        return;
    }

    int pivotIndex = partitionArray(array, startIndex, endIndex);
    quickSort(array, startIndex, pivotIndex - 1);
    quickSort(array, pivotIndex + 1, endIndex);
}

// Function to merge two sorted arrays
void mergeSortedArrays(int array[], int startIndex, int endIndex)
{
    int outputSize = (endIndex - startIndex) + 1;
    int* outputArray = new int[outputSize];

    int middleIndex = (startIndex + endIndex) / 2;
    int leftIndex = startIndex, rightIndex = middleIndex + 1, outputIndex = 0;

    while (leftIndex <= middleIndex && rightIndex <= endIndex)
    {
        if (array[leftIndex] <= array[rightIndex])
        {
            outputArray[outputIndex] = array[leftIndex];
            visualizeSorting(leftIndex, rightIndex);
            leftIndex++;
        }
        else
        {
            outputArray[outputIndex] = array[rightIndex];
            visualizeSorting(leftIndex, rightIndex);
            rightIndex++;
        }
        outputIndex++;
    }

    while (leftIndex <= middleIndex)
    {
        outputArray[outputIndex] = array[leftIndex];
        visualizeSorting(-1, leftIndex);
        leftIndex++;
        outputIndex++;
    }

    while (rightIndex <= endIndex)
    {
        outputArray[outputIndex] = array[rightIndex];
        visualizeSorting(-1, rightIndex);
        rightIndex++;
        outputIndex++;
    }

    for (int i = startIndex, j = 0; i <= endIndex; i++, j++)
    {
        array[i] = outputArray[j];
        visualizeSorting(i);
        SDL_Delay(15); // Delay for visualization
    }
    delete[] outputArray;
}

// Function to perform merge sort
void mergeSort(int array[], int startIndex, int endIndex)
{
    if (startIndex >= endIndex)
    {
        return;
    }

    int middleIndex = (startIndex + endIndex) / 2;
    mergeSort(array, startIndex, middleIndex);
    mergeSort(array, middleIndex + 1, endIndex);

    mergeSortedArrays(array, startIndex, endIndex);
}

// Function to perform bubble sort
void bubbleSort()
{
    for (int i = 0; i < ARRAY_SIZE - 1; i++)
    {
        for (int j = 0; j < ARRAY_SIZE - 1 - i; j++)
        {
            if (dataArray[j + 1] < dataArray[j])
            {
                // Swap values
                int temp = dataArray[j];
                dataArray[j] = dataArray[j + 1];
                dataArray[j + 1] = temp;

                visualizeSorting(j + 1, j, ARRAY_SIZE - i); // Visualize sorting
            }
            SDL_Delay(1); // Delay for visualization
        }
    }
}

// Function to perform insertion sort
void insertionSort()
{
    for (int i = 1; i < ARRAY_SIZE; i++)
    {
        int j = i - 1;
        int key = dataArray[i];
        while (j >= 0 && dataArray[j] > key)
        {
            dataArray[j + 1] = dataArray[j];
            j--;

            visualizeSorting(i, j + 1); // Visualize sorting
            SDL_Delay(5); // Delay for visualization
        }
        dataArray[j + 1] = key;
    }
}

// Function to perform selection sort
void selectionSort()
{
    int minIndex;
    for (int i = 0; i < ARRAY_SIZE - 1; i++)
    {
        minIndex = i;
        for (int j = i + 1; j < ARRAY_SIZE; j++)
        {
            if (dataArray[j] < dataArray[minIndex])
            {
                minIndex = j;
                visualizeSorting(i, minIndex); // Visualize sorting
            }
            SDL_Delay(1); // Delay for visualization
        }
        // Swap values
        int temp = dataArray[i];
        dataArray[i] = dataArray[minIndex];
        dataArray[minIndex] = temp;
    }
}

// Function to load the data array from the backup array
void loadArray()
{
    memcpy(dataArray, backupArray, sizeof(int) * ARRAY_SIZE);
}

// Function to generate a random array and save it to the backup array
void generateAndSaveRandomArray()
{
    unsigned int seed = static_cast<unsigned int>(time(NULL));
    srand(seed);
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        int randomValue = rand() % (SCREEN_HEIGHT);
        backupArray[i] = randomValue;
    }
}

// Function to execute the sorting visualizer
void executeSortingVisualizer()
{
    if (!initializeSDL())
    {
        cout << "SDL Initialization Failed.\n";
    }
    else
    {
        generateAndSaveRandomArray();
        loadArray();

        SDL_Event event;
        bool shouldQuit = false;
        while (!shouldQuit)
        {
            while (SDL_PollEvent(&event) != 0)
            {
                if (event.type == SDL_QUIT)
                {
                    shouldQuit = true;
                    isSortingComplete = false;
                }
                else if (event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                        case(SDLK_q):
                            shouldQuit = true;
                            isSortingComplete = false;
                            cout << "\nEXITING SORTING VISUALIZER.\n";
                            break;
                        case(SDLK_0):
                            generateAndSaveRandomArray();
                            isSortingComplete = false;
                            loadArray();
                            cout << "\nNEW RANDOM LIST GENERATED.\n";
                            break;
                        case(SDLK_1):
                            loadArray();
                            cout << "\nSELECTION SORT STARTED.\n";
                            isSortingComplete = false;
                            selectionSort();
                            isSortingComplete = true;
                            cout << "\nSELECTION SORT COMPLETE.\n";
                            break;
                        case(SDLK_2):
                            loadArray();
                            cout << "\nINSERTION SORT STARTED.\n";
                            isSortingComplete = false;
                            insertionSort();
                            isSortingComplete = true;
                            cout << "\nINSERTION SORT COMPLETE.\n";
                            break;
                        case(SDLK_3):
                            loadArray();
                            cout << "\nBUBBLE SORT STARTED.\n";
                            isSortingComplete = false;
                            bubbleSort();
                            isSortingComplete = true;
                            cout << "\nBUBBLE SORT COMPLETE.\n";
                            break;
                        case(SDLK_4):
                            loadArray();
                            cout << "\nMERGE SORT STARTED.\n";
                            isSortingComplete = false;
                            mergeSort(dataArray, 0, ARRAY_SIZE - 1);
                            isSortingComplete = true;
                            cout << "\nMERGE SORT COMPLETE.\n";
                            break;
                        case(SDLK_5):
                            loadArray();
                            cout << "\nQUICK SORT STARTED.\n";
                            isSortingComplete = false;
                            quickSort(dataArray, 0, ARRAY_SIZE - 1);
                            isSortingComplete = true;
                            cout << "\nQUICK SORT COMPLETE.\n";
                            break;
                        case(SDLK_6):
                            loadArray();
                            cout << "\nHEAP SORT STARTED.\n";
                            isSortingComplete = false;
                            heapSort(dataArray, ARRAY_SIZE);
                            isSortingComplete = true;
                            cout << "\nHEAP SORT COMPLETE.\n";
                            break;
                    }
                }
            }
            visualizeSorting();
        }
        cleanUpSDL();
    }
}

// Function to show the controls and instructions
bool showControls()
{
    cout << "WARNING: Giving repetitive commands may cause latency and the visualizer may behave unexpectedly. Please give a new command only after the current command's execution is done.\n\n"
         << "Available Controls inside Sorting Visualizer:-\n"
         << "    Use 0 to Generate a different randomized list.\n"
         << "    Use 1 to start Merge Sort Algorithm.\n" 
         << "    Use 2 to start Insertion Sort Algorithm.\n" 
         << "    Use 3 to start Bubble Sort Algorithm.\n" 
         << "    Use 4 to start Selection Sort Algorithm.\n" 
         << "    Use 5 to start Quick Sort Algorithm.\n"
         << "    Use 6 to start Heap Sort Algorithm.\n"
         << "    Use q to exit out of Sorting Visualizer\n\n"
         << "PRESS ENTER TO START SORTING VISUALIZER...\n\n"
         << "Or type -1 and press ENTER to quit the program.";

    string input;
    getline(cin, input);
    if (input == "-1")
    {
        return false;
    }
    return true;
}

// Function to display the introduction message
void showIntroduction()
{
    cout << "==============================Sorting Visualizer==============================\n\n"
         << "Visualization of different sorting algorithms in C++ with SDL2 Library. A sorting algorithm is an algorithm that puts the elements of a list in a certain order. While there are a large number of sorting algorithms, in practical implementations a few algorithms predominate.\n"
         << "In this implementation of sorting visualizer, we'll be looking at some of these sorting algorithms and visually comprehend their working.\n"
         << "The sorting algorithms covered here are Selection Sort, Insertion Sort, Bubble Sort, Merge Sort, Quick Sort and Heap Sort.\n"
         << "The list size is fixed to 130 elements. You can randomize the list and select any type of sorting algorithm to call on the list from the given options. Here, all sorting algorithms will sort the elements in ascending order. The sorting time being visualized for an algorithm is not exactly same as their actual time complexities. The relatively faster algorithms like Merge Sort, etc. have been delayed so that they could be properly visualized.\n\n"
         << "Press ENTER to show controls...";

    string input;
    getline(cin, input);
    if (input == "\n")
    {
        return;
    }
}

int main(int argc, char* args[])
{
    showIntroduction();

    while (true)
    {
        cout << '\n';
        if (showControls())
            executeSortingVisualizer();
        else
        {
            cout << "\nEXITING PROGRAM.\n";
            break;
        }
    }

    return 0;
}

