@x = global i32 6
@y = global i32 7

define void @_start() {
  store volatile i32 2, ptr @x
  store volatile i32 3, ptr @y

  %vx = load volatile i32, ptr @x
  %vy = load volatile i32, ptr @y
  %sum = add i32 %vx, %vy

  %is_lte = icmp sle i32 %sum, 10
  %is_lte_i32 = zext i1 %is_lte to i32
  store volatile i32 %is_lte_i32, ptr @x
  
  ret void
}
