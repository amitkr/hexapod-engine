 
leg0 =
 
([[ ct0, -st0, 0, ct0*r0]
[ st0,  ct0, 0, r0*st0]
[   0,    0, 1,     d0]
[   0,    0, 0,      1]])
 
 
leg1 =
 
[ ct1, -ca1*st1,  sa1*st1, ct1*r1]
[ st1,  ca1*ct1, -ct1*sa1, r1*st1]
[   0,      sa1,      ca1,     d1]
[   0,        0,        0,      1]
 
 
leg2 =
 
[ ct2, -st2, 0, ct2*r2]
[ st2,  ct2, 0, r2*st2]
[   0,    0, 1,     d2]
[   0,    0, 0,      1]
 
 
leg3 =
 
[ ct3, -st3, 0,  0]
[ st3,  ct3, 0,  0]
[   0,    0, 1, d3]
[   0,    0, 0,  1]
 
 
coxa =
 
[ ct0, -st0, 0, ct0*r0]
[ st0,  ct0, 0, r0*st0]
[   0,    0, 1,     d0]
[   0,    0, 0,      1]
 
 
femur =
 
[ ct0*ct1 - st0*st1, - ca1*ct0*st1 - ca1*ct1*st0, ct0*sa1*st1 + ct1*sa1*st0, ct0*r0 + ct0*ct1*r1 - r1*st0*st1]
[ ct0*st1 + ct1*st0,   ca1*ct0*ct1 - ca1*st0*st1, sa1*st0*st1 - ct0*ct1*sa1, r0*st0 + ct0*r1*st1 + ct1*r1*st0]
[                 0,                         sa1,                       ca1,                          d0 + d1]
[                 0,                           0,                         0,                                1]
 
 
tibia =
 
[ ct2*(ct0*ct1 - st0*st1) - st2*(ca1*ct0*st1 + ca1*ct1*st0), - ct2*(ca1*ct0*st1 + ca1*ct1*st0) - st2*(ct0*ct1 - st0*st1), ct0*sa1*st1 + ct1*sa1*st0, ct0*r0 + d2*(ct0*sa1*st1 + ct1*sa1*st0) + ct2*r2*(ct0*ct1 - st0*st1) + ct0*ct1*r1 - r1*st0*st1 - r2*st2*(ca1*ct0*st1 + ca1*ct1*st0)]
[ st2*(ca1*ct0*ct1 - ca1*st0*st1) + ct2*(ct0*st1 + ct1*st0),   ct2*(ca1*ct0*ct1 - ca1*st0*st1) - st2*(ct0*st1 + ct1*st0), sa1*st0*st1 - ct0*ct1*sa1, r0*st0 - d2*(ct0*ct1*sa1 - sa1*st0*st1) + ct2*r2*(ct0*st1 + ct1*st0) + ct0*r1*st1 + ct1*r1*st0 + r2*st2*(ca1*ct0*ct1 - ca1*st0*st1)]
[                                                   sa1*st2,                                                     ct2*sa1,                       ca1,                                                                                                       d0 + d1 + ca1*d2 + r2*sa1*st2]
[                                                         0,                                                           0,                         0,                                                                                                                                   1]
 
 
foot =
 
[ - ct3*(st2*(ca1*ct0*st1 + ca1*ct1*st0) - ct2*(ct0*ct1 - st0*st1)) - st3*(ct2*(ca1*ct0*st1 + ca1*ct1*st0) + st2*(ct0*ct1 - st0*st1)), st3*(st2*(ca1*ct0*st1 + ca1*ct1*st0) - ct2*(ct0*ct1 - st0*st1)) - ct3*(ct2*(ca1*ct0*st1 + ca1*ct1*st0) + st2*(ct0*ct1 - st0*st1)), ct0*sa1*st1 + ct1*sa1*st0, ct0*r0 + d2*(ct0*sa1*st1 + ct1*sa1*st0) + d3*(ct0*sa1*st1 + ct1*sa1*st0) + ct2*r2*(ct0*ct1 - st0*st1) + ct0*ct1*r1 - r1*st0*st1 - r2*st2*(ca1*ct0*st1 + ca1*ct1*st0)]
[ct3*(st2*(ca1*ct0*ct1 - ca1*st0*st1) + ct2*(ct0*st1 + ct1*st0)) + st3*(ct2*(ca1*ct0*ct1 - ca1*st0*st1) - st2*(ct0*st1 + ct1*st0)), ct3*(ct2*(ca1*ct0*ct1 - ca1*st0*st1) - st2*(ct0*st1 + ct1*st0)) - st3*(st2*(ca1*ct0*ct1 - ca1*st0*st1) + ct2*(ct0*st1 + ct1*st0)), sa1*st0*st1 - ct0*ct1*sa1, r0*st0 - d2*(ct0*ct1*sa1 - sa1*st0*st1) - d3*(ct0*ct1*sa1 - sa1*st0*st1) + ct2*r2*(ct0*st1 + ct1*st0) + ct0*r1*st1 + ct1*r1*st0 + r2*st2*(ca1*ct0*ct1 - ca1*st0*st1)]
[ ct2*sa1*st3 + ct3*sa1*st2, ct2*ct3*sa1 - sa1*st2*st3, ca1, d0 + d1 + ca1*d2 + ca1*d3 + r2*sa1*st2]
[ 0, 0, 0, 1]
 
