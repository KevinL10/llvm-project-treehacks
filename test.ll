@x = global i32 6
@y = global i32 7

define void @_start() {
  store volatile i32 2, ptr @x
  store volatile i32 17, ptr @y

  %vx = load volatile i32, ptr @x
  %vy = load volatile i32, ptr @y
  %difference = sub i32 %vx, %vy

  store volatile i32 %difference, ptr @x
  
  ret void
}
