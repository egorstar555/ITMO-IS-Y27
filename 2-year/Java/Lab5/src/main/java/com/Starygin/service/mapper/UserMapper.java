package com.Starygin.service.mapper;

import com.Starygin.service.dto.UserDto;
import com.Starygin.model.entity.User;
import org.springframework.stereotype.Component;

@Component
public class UserMapper implements Mapper<User, UserDto> {

    @Override
    public User toEntity(UserDto dto) {
        return new User(dto.name(), dto.password(), dto.role());
    }

    @Override
    public UserDto toDto(User entity) {
        return new UserDto(entity.getId(), entity.getName(), entity.getPassword(), entity.getRole());
    }
}
