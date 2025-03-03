#ifndef BINOMIAL_PRICER_DLL_HPP
#define BINOMIAL_PRICER_DLL_HPP

// Export macro definition for the Binomial Pricer DLL.
// When building the DLL, define BINOMIAL_PRICER_DLL_EXPORTS to export symbols; otherwise, they are imported.
#ifdef BINOMIAL_PRICER_DLL_EXPORTS
#define BINOMIAL_PRICER_API __declspec(dllexport)
#else
#define BINOMIAL_PRICER_API __declspec(dllimport)
#endif

#include "pch.h" // Inclusion of the precompiled header for faster compilation

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief Prices an option using the binomial CRR model.
     *
     * This function calculates the price of an option by constructing a binomial tree using the
     * Cox-Ross-Rubinstein method. It is designed to support both call and put options as well as
     * European and American styles.
     *
     * @param S The current price of the underlying asset.
     * @param K The strike price of the option.
     * @param T The time to maturity in years.
     * @param r The risk-free interest rate.
     * @param sigma The volatility of the underlying asset.
     * @param q The dividend yield of the underlying asset.
     * @param optionType The type of the option: 0 indicates Call, 1 indicates Put.
     * @param optionStyle The style of the option: 0 for European, 1 for American.
     * @param calculationDate The calculation date in the format "YYYY-MM-DD". If empty, the current date is used.
     * @param binomialSteps The number of steps to be used in constructing the binomial tree.
     * @return The calculated option price as a double.
     *
     * @details
     * The function discretizes the option's life into a series of steps using a binomial tree model.
     * It then calculates possible asset prices at each node and applies backward induction to determine
     * the present value of the option. The __stdcall calling convention ensures compatibility with various
     * systems and languages.
     */
    BINOMIAL_PRICER_API double __stdcall PriceOptionBinomial(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int binomialSteps);

    /**
     * @brief Computes the Greeks of an option using the binomial CRR model.
     *
     * This function calculates key sensitivity measures (Greeks) such as delta, gamma, vega, theta, and rho
     * by employing finite difference approximations within the binomial pricing framework. The results are
     * returned via pointer parameters.
     *
     * @param S The current price of the underlying asset.
     * @param K The strike price of the option.
     * @param T The time to maturity in years.
     * @param r The risk-free interest rate.
     * @param sigma The volatility of the underlying asset.
     * @param q The dividend yield of the underlying asset.
     * @param optionType The type of the option: 0 indicates Call, 1 indicates Put.
     * @param optionStyle The style of the option: 0 for European, 1 for American.
     * @param calculationDate The calculation date in the format "YYYY-MM-DD". If empty, the current date is used.
     * @param binomialSteps The number of steps in the binomial tree.
     * @param delta Pointer to a double where the computed Delta will be stored.
     * @param gamma Pointer to a double where the computed Gamma will be stored.
     * @param vega Pointer to a double where the computed Vega will be stored.
     * @param theta Pointer to a double where the computed Theta will be stored.
     * @param rho Pointer to a double where the computed Rho will be stored.
     *
     * @details
     * By perturbing input parameters and re-evaluating the option price, this function estimates the
     * sensitivity of the option's price to changes in market conditions. These calculations are vital for
     * risk management and for constructing hedging strategies.
     */
    BINOMIAL_PRICER_API void __stdcall ComputeOptionGreeksBinomial(
        double S, double K, double T, double r, double sigma, double q,
        int optionType, int optionStyle, const char* calculationDate,
        int binomialSteps,
        double* delta, double* gamma, double* vega, double* theta, double* rho);

#ifdef __cplusplus
}
#endif

#endif // BINOMIAL_PRICER_DLL_HPP
