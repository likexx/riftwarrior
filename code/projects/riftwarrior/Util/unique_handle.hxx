///
/// Talk is cheap. Show me the code.    - Linus Torvalds
///
/// Author: WiNGC (wingc@wingc.net)
/// All rights reserved
///

///
/// This is a template to accomplish auto resource release.
/// When we use it we have to use a type and the type specific traits class
/// which wraps the type's default invlaid value and the how to close it.
/// Most of this auto management concept and code are from Kenny Kerr's
/// <C++ and the Windows API> http://msdn.microsoft.com/en-us/magazine/hh288076.aspx
///
/// E.g. On Windows platform, we want to wrap file handle HANDLE and its closer
/// CloseHandle() we just need to define
/// struct HandleTraits
/// {
///     static HANDLE invalid() throw() { return INVALID_HANDLE_VALUE; };
///     static void close(HANDLE value) throw() { ::CloseHandle(value); };
/// }
/// and then
/// typedef unqiue_handle<HANDLE, HandleTraits> unique_file;
/// unique_file file;
/// file = ::CreateFile2(...);
///
/// Note: 
///     - Windows platform specific code I always we UpperCamelCased naming convention.
///     - The unique_handle tempate itself is platform independent I place this under Util
//         and follow lower_underscore_cased naming.
//      - The platform specific code like the unique_file definition is placed under Platform/Win32.

template<typename T, typename Traits>
class unique_handle
{
    // Copy constructor not allowed.
    unique_handle(unique_handle const&);
    // Assignment not allowed.
    unique_handle& operator=(unique_handle const&);
    void close() throw()
    {
        if (is_valid())
        {
            Traits::close(m_value);
            m_value = Traits::invalid();
        }
    }
    T m_value;
public:
    explicit unique_handle(T value = Traits::invalid()) throw() : m_value(value) {};
    ~unique_handle() throw() { close(); };
    bool is_valid() throw() { return (m_value != Traits::invalid()); };
    bool reset(T value = Traits::invalid()) throw()
    {
        if (is_valid())
        {
            close();
        }
        m_value = value;
        return is_valid();
    }
    unique_handle& operator=(T value)
    {
        reset(value);
        return *this;
    }
    T get() const throw() { return m_value; }
    T release() throw()
    {
        T value = m_value;
        m_value = Traits::invalid();
        return value;
    }
};
