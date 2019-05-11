#!/usr/bin/python
# -*- coding: utf-8 -*-

class Event(dict):
    """Event subscription with event names as keys

    A dict of callable values. Calling an instance of this
    with a key will call the associated item

   Example Usage:
    >>> def f(x):
    ...     print 'f(%s)' % x
    >>> e = Event()
    >>> e()
    >>> e.update({'f': f})
    >>> e('f')
    f(123)
    >>> e.pop('f')
    >>> e()

    """
    def __call__(self, uid, *args, **kwargs):
        if uid in self:
            self[uid](*args, **kwargs)

    def __repr__(self):
        return "Event(%s)" % dict.__repr__(self)