package com.Starygin.service.dto;

import com.Starygin.model.entity.USER_ROLE;

public record UserDto(Long id, String name, String password, USER_ROLE role) {
}