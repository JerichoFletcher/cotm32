package cotm32_test_pkg;

function void assert_eq(
  input int expected,
  input int actual
);
  assert (expected == actual)
    else begin
      $error("Assertion failed (expected %h, actual %h)", expected, actual);
      $finish;
    end
endfunction

endpackage : cotm32_test_pkg