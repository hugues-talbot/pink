set PROBAS(randx) 237465
set PROBAS(divis) 2147483648.0 
# = 2**31, cycle of rand()

# Functionality : [0, 1[ uniform probability distribution
#-----------------------------------
proc Prob {} {
  global PROBAS
  set PROBAS(randx) [expr $PROBAS(randx) * 1103515245 + 12345]  
  return [expr ($PROBAS(randx) & 0x7fffffff) / $PROBAS(divis)]  
}

# Functionality : Reset probability
#-----------------------------------
proc ResetProb {num} {
  global PROBAS
  set PROBAS(randx) $num
}
