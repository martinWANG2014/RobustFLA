#!/bin/python
import numpy as np
import pynverse
import scipy.special as sp


###############################################################################
# the var sigma 1 function
# input:
#       sigma   the standard deviation of random variables
#       alpha   the absolute of lower bound far from mean of random variables
#       beta    the upper bound far from mean of random variables
# output:
#       the probability for -alpha<= y <= beta
###############################################################################
def var_sigma_1(sigma, alpha, beta):
    return 0.5 * (sp.erf(beta / (np.sqrt(2) * sigma)) + sp.erf(alpha / (np.sqrt(2) * sigma)))


###############################################################################
# the inverse var sigma 1 function
# input:
#       alpha   the absolute of lower bound for random variables
#       beta    the upper bound for random variables
#       gamma   the probability for -alpha <= y <= beta
# output:
#       sigma   the standard deviation of random variables
###############################################################################
def inverse_var_sigma_1(alpha, beta, gamma):
    res_sigma = pynverse.inversefunc(var_sigma_1, args=(alpha, beta), domain=(0, 10000))
    return res_sigma(gamma)


###############################################################################
# the var sigma 2 function
# input:
#       sigma   the standard deviation of random variables
#       alpha   the absolute of lower bound far from mean of  random variables
#       beta    the upper bound far from mean of  random variables
#       w1      the related position of mean for the random variable in left part
#       w2      the related position of mean for the random variable in right part
#       p       the probability of random variables in left part
# output:
#       the probability for -alpha <= y <= beta
###############################################################################
def var_sigma_2(sigma, alpha, beta, w1, w2, p):
    return 0.5 * p * (
    sp.erf((1 - w1) * alpha / (sigma * np.sqrt(2))) + sp.erf((1 + w1) * alpha / (sigma * np.sqrt(2)))) + 0.5 * (
    1 - p) * (sp.erf((1 - w2) * beta / (sigma * np.sqrt(2))) + sp.erf((1 + w2) * beta / (sigma * np.sqrt(2))))


###############################################################################
# the var sigma 2 function
# input:
#       gamma   the probability for -alpha<= y <= beta
#       alpha   the absolute of lower bound for random variables
#       w1      the related position of mean for the random variable in left part
#       beta    the upper bound for random variables
#       w2      the related position of mean for the random variable in right part
#       p       the probability of random variables in left part
# output:
#       sigma   the standard deviation of random variables
###############################################################################
def inverse_var_sigma_2(alpha, beta, gamma, w1, w2, p):
    res_sigma = pynverse.inversefunc(var_sigma_2, args=(alpha, beta, w1, w2, p), domain=(0, 10000))
    return res_sigma(gamma)
