namespace APowers {

/// @brief Safely releases the given pointer to a windows component.
///
/// @tparam T The type of component to release.
/// @param ppT Pointer to a point to the component will be set to nullptr after release.
template <class T>
void safeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = nullptr;
    }
}

} // namespace APowers
