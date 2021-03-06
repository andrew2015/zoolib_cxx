Naming Convention
=================
Prefixes Designate Scope
------------------------
Most of my own confusion when reading unfamiliar C++ is determining from which scope a name is drawn.

We've got parameters, local and instance variables, and class, file and namespace statics to
distinguish. ZooLib uses name prefixes to make distinct the scope in which a name is defined.

- `f` instance variables. (f is an abbreviation of 'field', cf MacApp/Taligent/Object Pascal).
	They're almost always private.
- `p` private functions and methods.
- `s` static variables and functions. These are class or namespace static, and
	are publicly accessible.
- `sp` static private variables and functions. If file static then obviously they're not
	publicly visible. If class static, then they're marked protected if intended for use by
	subclasses, private if only for use by the class itself.
- `i` input parameter. This is the most common type of parameter and probably conforms most
	closely to the classic definition of a parameter. Its value at entry will very likely
	affect what the function does. Its value in the calling scope will not be changed by the
	function's execution. If the function cannot operate without its value, then it's a compulsory
	parameter and is passed by value or const reference. If the parameter is optional it's passed
	by const pointer, so the caller can pass nullptr indicating that no value is available.
- `o` output parameter. Output parameters let a function return multiple values. Its value on
	entry will not affect the function's execution and the function takes full responsibility for
	giving it an appropriate value before returning. If the parameter is compulsory it is passed
	by non-const reference. If optional, by non-const pointer.
- `io` input/output parameter. An input/output parameter's value at entry may affect the function's
	execution and should thus have a sensible value. Its value in the calling scope may be changed
	by the function's execution. If compulsory, pass by non-const reference. If optional by
	non-const pointer, just as with output parameters.
- `e` enumeration value. Enumerations tend to be class public, so we don't encode the scope
	itself, just that it is an enumeration value.
- `E` enumeration type

Names
-----
Types are nouns or noun phrases; functions are verbs or verb phrases.

Names that are phrases are CamelCased, each word in the phrase has its first letter upper cased.
If there is a prefix, the prefix is lower cased and the 'meat' starts with an upper case letter,
the remainder being CamelCased.

For non-class types I'll often use a `_t` suffix, and for template types/classes/functions
a `_T` suffix.

Classes and method names have an upper case initial letter.

Local variables have a lower case initial letter. I often use a the prefix, but the goal is just
to make it read well.

Pretty much everything else has a scope prefix.

Compound Names
--------------
This may be the most controversial stylistic decision, but I've found it helpful.

Much of ZooLib consists of abstract interfaces and a suite of concrete implementations. In such
situations the base is named with a noun or noun phrase (CamelCased of course). Concrete
implementations are named with the base name, an underscore, and a distinguishing noun or noun
phrase. This approach keeps hierarchies together when their members are sorted lexicographically.
It also provides a strong hint of a type's likely compatibility – if you see an XxxYyy_Zzz it can
generally be passed to a function expecting an XxxYyy.

For types that are related but not type-compatible I use reverse English order noun phrases, so
XxxYyy and XxxZzz are probably part of an Xxx package.

The goal is not to be rigidly prescriptive, but to make several hundreds of names somewhat
predictable and intelligible.
