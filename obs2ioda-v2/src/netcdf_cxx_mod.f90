module netcdf_cxx_mod
    use iso_c_binding, only : c_char, c_null_char, c_null_ptr, c_int
    use f_c_string_t_mod, only : f_c_string_t
    use f_c_string_1D_t_mod, only : f_c_string_1D_t
    use netcdf_cxx_i_mod
    use netcdf, only : NF90_INT, NF90_INT64, NF90_REAL
    implicit none
    public

contains

    function netcdfCreate(path, netcdfID)
        character(len = *), intent(in) :: path
        integer(c_int), intent(inout) :: netcdfID
        integer(c_int) :: netcdfCreate
        type(f_c_string_t) :: f_c_string_path
        type(c_ptr) :: c_path

        c_path = f_c_string_path%to_c(path)
        netcdfCreate = c_netcdfCreate(c_path, netcdfID)
    end function netcdfCreate

    function netcdfClose(netcdfID)
        integer(c_int), value, intent(in) :: netcdfID
        integer(c_int) :: netcdfClose
        netcdfClose = c_netcdfClose(netcdfID)
    end function netcdfClose


end module netcdf_cxx_mod