// Custom Lib

int c_atoi(char *str)
{ //custom atoi function so I don't need the stdlib
  int res = 0;
  for (int i = 0; str[i] != '\0'; i++)
    res = res * 10 + str[i] - '0';
  return res;
  //Credit : https://www.geeksforgeeks.org/write-your-own-atoi/
}