#include <iostream>
#include <vector>
#include <algorithm>

// Function to get the maximum price using dynamic programming
int getMaxPrice(int dimx, int dimy, std::vector<std::vector<int>> dp)
{
    // Iterate over all possible subproblems
    for (int i = 1; i <= dimx; ++i)
    {
        for (int j = 1; j <= dimy; ++j)
        {
            // Iterate all possible cuts
            for (int k = 1; k < j; ++k)
                if (dp[i][j] < dp[i][k] + dp[i][j - k])
                {
                    dp[i][j] = dp[i][k] + dp[i][j - k];
                }

            for (int k = 1; k < i; ++k)
                if (dp[i][j] < dp[k][j] + dp[i - k][j])
                {
                    dp[i][j] = dp[k][j] + dp[i - k][j];
                }
        }
    }
    // Return the maximum price for the original problem
    return dp[dimx][dimy];
}

int main()
{
    int dimx, dimy, ntypes;

    // Input dimensions of the plate
    scanf("%d", &dimx);
    scanf("%d", &dimy);

    // Create a 2D vector to store the results of subproblems
    std::vector<std::vector<int>> dp(dimx + 1, std::vector<int>(dimy + 1, 0));

    // Input the number of piece types
    scanf("%d", &ntypes);

    // Input dimensions and prices of each piece type
    for (int i = 0; i < ntypes; ++i)
    {
        int x, y, price;

        scanf("%d", &x);
        scanf("%d", &y);
        scanf("%d", &price);

        if (x <= dimx && y <= dimy && price > dp[x][y])
            dp[x][y] = price;

        if (y <= dimx && x <= dimy && price > dp[y][x])
            dp[y][x] = price;
    }

    // Calculate and output the maximum price
    int maxPrice = getMaxPrice(dimx, dimy, dp);

    std::cout << maxPrice << std::endl;
}
