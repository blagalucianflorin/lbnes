//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#ifndef NEMULATOR_MACROS_H
#define NEMULATOR_MACROS_H

#define IS_BETWEEN(l, v, r) (static_cast <long long> (l) <= static_cast <long long> (v) && static_cast <long long> (v) \
    <= static_cast <long long> (r))
#define IS_STRICT_BETWEEN(l, v, r) ((l) < (v) && (v) < (r))

#endif //NEMULATOR_MACROS_H
