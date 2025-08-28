package com.starygin.ownersms.service.mapper;

import com.starygin.ownersms.service.dto.OwnerDto;
import com.starygin.ownersms.entity.Owner;
import org.springframework.stereotype.Component;

@Component
public class OwnerMapper implements Mapper<Owner, OwnerDto> {

    @Override
    public Owner toEntity(OwnerDto dto) {
        Owner owner = new Owner();
        owner.setId(dto.id());
        owner.setName(dto.name());
        owner.setBirthday(dto.birthday());
        owner.setUserId(dto.userId());

        return owner;
    }

    @Override
    public OwnerDto toDto(Owner entity) {
        return new OwnerDto(entity.getId(), entity.getName(), entity.getBirthday(), entity.getUserId());
    }
}
