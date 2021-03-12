var digit = /^[0-9]+$/;

function isDigit(str) {
    assertString(str);
    return digit.test(str);
}
      