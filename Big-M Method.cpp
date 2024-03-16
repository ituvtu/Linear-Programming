#include<iostream>
#include<iomanip>
#define M 1000
#define MAX 100
using namespace std;

long double A[MAX][MAX]; // Declare a global 2D array to hold the tableau.
int t=1;
// Function to print the simplex tableau with borders and iteration numbers.
void print1(int m, int n, int p)
{
    static int iteration = 0; // Static variable to track the number of iterations.
    iteration++; // Increment the iteration count on each function call.

    // Print the iteration number and prepare the table header.
    cout << "Iteration #" << iteration << ":\n";
    // Header border
    cout << "+-----+-------+-------+-------+";
    for (int i = 0; i < n + p; i++) {
        cout << "-------+";
    }
    cout << endl;

    // Table headers for indices, base variables, coefficients, and variables.
    cout << "|  i  |  Base |   C   |";
    for (int i = 1; i <= n + p; i++) {
        cout << "   X" << i << "  |";
    }
    cout << "   B   |" << endl; // Column for the RHS values (B).
    // Underline the header
    cout << "+-----+-------+-------+";
    for (int i = 0; i < n + p; i++) {
        cout << "-------+";
    }
    cout << endl;

    // Print the tableau rows.
    for (int i = 0; i <= m + 1; i++) {
        cout << "|";
        if (i <= m) {
            cout << setw(5) << i; // Row number.
        }
        else {
            cout << "     ";
        }
        cout << "|";
        if (i < m + 1) {
            cout << "  x" << setw(3) << A[i][n + p + 2] << " |"; // Base variable for the row.
        }
        else {
            cout << "     |"; // Last row for objective function.
        }
        for (int j = 1; j <= n + p + 2; j++) {
            if (j == n + p + 1) {
                cout << setw(7) << A[i][j] << "|"; // RHS value for constraints.
            }
            else if (i == m + 1 && j == n + p + 2) {
                // Skip for the objective function's last cell.
            }
            else {
                cout << setw(7) << A[i][j] << "|"; // Coefficients and objective function values.
            }
        }
        cout << endl;
    }
    // Bottom border of the table.
    cout << "+-----+-------+-------+";
    for (int i = 0; i < n + p; i++) {
        cout << "-------+";
    }
    cout << endl;
}

// Function to adjust the objective function by subtracting M times the artificial variables.
void simpfunc(int m, int n, int p)
{
    int j, i;
    for (j = 1; j <= n + p + 1; j++)
    {
        for (i = 1; i <= m; i++)
        {
            A[m + 1][j] = A[m + 1][j] - M * A[i][j];
        }
    }
}

// Function to input the constraints, objective function, and determine if maximization or minimization.
void input(int m, int n, int p)
{
    int i, j, count = 1;
    char r;
    double x;
    cout << "\nMaximize (Y/N)  :  ";
    cin >> r;
    if ((r == 'Y') || (r == 'y'))
    {
        t = -1; // If maximizing, flip the sign of the objective function coefficients.
    }
    // Input the constraints.
    for (i = 1; i <= m; i++)
    {
        cout << "\nCONSTRAINT " << i << "  :  \n";
        cout << "\nWhether type \">=\" (Y/N)   :  ";
        cin >> r;
        for (j = 1; j <= n; j++) {
            cout << "Coefficient of x" << j << "  :  ";
            cin >> x;
            A[i][j] = x; // Input the coefficients of the variables in the constraint.
        }
        if (r == 'y' || r == 'Y')
        {
            for (j = 1; j <= p; j++)
            {
                A[i][n + j] = 0;
            }
            A[i][n + count] = -1; // Marking slack/surplus variables for '>=', set to -1 for slack.
            count++;
        }
        else
        {
            for (j = 1; j <= p; j++)
            {
                A[i][j + n] = 0; // For '<=' constraints, slack variables are not used.
            }
        }
        cout << "Constant  :  ";
        cin >> A[i][n + p + 1]; // The RHS value of the constraint.
    }

    // Input for the objective function.
    cout << "\nInput for the function  :  \n";
    for (j = 1; j <= n; j++)
    {
        cout << "Coefficient of x" << j << "  :  ";
        cin >> x;
        A[m + 1][j] = x * t; // Adjust the sign based on maximization or minimization.
    }
    for (j = 1; j <= p; j++)
    {
        A[i][j + n] = 0; // Initialize artificial variables in the objective function row.
    }
    cout << "Value of d  :  ";
    cin >> A[m + 1][n + 1]; // Input the constant term of the objective function, if any.
    print1(m, n, p); // Print the initial tableau.
    simpfunc(m, n, p); // Adjust the objective function for the simplex method.
}

// Function to find the pivot column based on the smallest coefficient in the objective function row.
int pivcol(int m, int n, int p)
{
    int i, pos = 1;
    double min = A[m + 1][1];
    for (i = 1; i <= n + p; i++)
    {
        if (min > A[m + 1][i])
        {
            min = A[m + 1][i];
            pos = i; // Update the position of the smallest coefficient.
        }
    }
    return pos; // Return the position of the pivot column.
}

// Function to find the pivot row using the minimum ratio test (excluding negative ratios).
int pivrow(int m, int n, int p, int pos)
{
    int i, piv = 1;
    double min = (A[1][n + p + 1] / A[1][pos]);
    for (i = 1; i <= m; i++)
    {
        if (A[i][pos] > 0) // Ensure the denominator is positive for a valid ratio.
        {
            double ratio = A[i][n + p + 1] / A[i][pos];
            if (min > ratio)
            {
                min = ratio;
                piv = i; // Update the pivot row based on the smallest valid ratio.
            }
        }
    }
    return piv; // Return the position of the pivot row.
}

// Function to perform one iteration of the simplex algorithm (optimization step).
void optimise(int m, int n, int p)
{
    print1(m, n, p); // Print the current tableau.
    int i, j, pivr, pivc;
    pivc = pivcol(m, n, p); // Find the pivot column.
    pivr = pivrow(m, n, p, pivc); // Find the pivot row.

    // Perform the row operations to form the next tableau.
    for (i = 1; i <= m + 1; i++)
    {
        for (j = 1; j <= n + p + 1; j++)
        {
            if ((i != pivr) && (j != pivc)) // For all cells not in the pivot row or column.
            {
                A[i][j] = (((A[i][j] * A[pivr][pivc]) - (A[i][pivc] * A[pivr][j])) / A[pivr][pivc]);
            }
        }
    }

    // Adjust the pivot column and row to reflect the basic variable change.
    for (i = 1; i <= m + 1; i++)
    {
        if (pivr != i) // For rows other than the pivot row.
        {
            A[i][pivc] = -A[i][pivc] / A[pivr][pivc];
        }
    }
    for (j = 1; j <= n + p + 1; j++)
    {
        if (pivc != j) // For columns other than the pivot column.
        {
            A[pivr][j] = A[pivr][j] / A[pivr][pivc];
        }
    }

    // Set the pivot element to its inverse to maintain the identity property.
    A[pivr][pivc] = 1 / A[pivr][pivc];
    // Swap the labels for the basic variable.
    double temp = A[pivr][n + p + 2];
    A[pivr][n + p + 2] = A[0][pivc];
    A[0][pivc] = temp;
}

// Function to print the final results, including basic variables and the maximum value.
void result(int m, int n, int p)
{
    cout << "\nThe Basic Variables  :  \n";
    for (int i = 1; i <= m; i++)
    {
        cout << "\nx" << A[i][n + p + 2] << "  =  " << A[i][n + p + 1]; // Print each basic variable and its value.
    }
    float value = 1+ A[m + 1][n + 1];
    if (t == 1) {
        cout << "\n\nMin Value  :  " << (int)value<< endl << endl;
    }
    else {
        cout << "\n\nMax Value  :  " << (int)value << endl << endl; // Print the objective function's value.
    }
}

// Function to check for alternate solutions by examining zero coefficients in the objective function row.
void altsimplex(int m, int n, int p)
{
    int i = 1, j = 1, z = 1;
    for (j = 1; j <= n + p; j++)
    {
        if (A[m + 1][j] == 0) // If any coefficient in the objective function row is zero.
        {
            z = 0; // Indicates the potential for alternate solutions.
        }
    }
    if (z == 0) // If there's a potential for alternate solutions.
    {
        optimise(m, n, p); // Perform another optimization step.
        print1(m, n, p); // Print the tableau.
        cout << "\n\nAlternate Solution   : \n";
        result(m, n, p); // Print the results, which may indicate alternate solutions.
    }
}

// The main simplex function that drives the algorithm, iterating until an optimal solution is found.
int simplex(int m, int n, int p)
{
    int j, x = 1, z = 0;
    for (j = 1; j <= n + p; j++)
    {
        if (A[m + 1][j] <= 0) // Check if any coefficients in the objective function row are negative.
        {
            z = 1; // Indicates the need for further optimization.
        }
    }
    while (z == 1) // While further optimization is needed.
    {
        optimise(m, n, p); // Perform an optimization step.
        z = 0; // Reset the flag.
        for (j = 1; j <= n + p; j++)
        {
            if (A[m + 1][j] < 0) // Check again for negative coefficients.
            {
                z = 1; // Indicates the need for further optimization.
            }
        }
    }
    print1(m, n, p); // Print the final tableau.
    result(m, n, p); // Print the final results.
    altsimplex(m, n, p); // Check for alternate solutions.
    return 0; // Indicate successful completion.
}

// The main entry point for the program, which sets up the problem and initiates the simplex algorithm.
int main()
{
    int i, j, n, m, p;
    cout << "\nEnter the number of variables of the function  :  ";
    cin >> n; // Input the number of variables.
    cout << "\nEnter the number of constraints  :  ";
    cin >> m; // Input the number of constraints.
    cout << "\nEnter the number of '>= sign'  :  ";
    cin >> p; // Input the number of '>=' constraints.
    input(m, n, p); // Input the problem data.
    print1(m, n, p); // Print the initial tableau.
    for (i = 1; i <= n + p; i++)
    {
        A[0][i] = i; // Label the columns.
    }
    for (j = 1; j <= m; j++)
    {
        A[j][n + p + 2] = i++; // Label the rows for basic variables.
    }
    print1(m, n, p); // Print the tableau with labels.
    simplex(m, n, p); // Start the simplex algorithm.
    return 0; // Exit the program.
}
