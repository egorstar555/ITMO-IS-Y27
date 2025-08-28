package com.Starygin.service.mapper;

import com.Starygin.data.entity.Owner;
import com.Starygin.data.entity.Pet;
import com.Starygin.service.dto.OwnerSimpleDto;
import com.Starygin.service.dto.PetDto;
import com.Starygin.service.dto.PetSimpleDto;

import java.util.ArrayList;
import java.util.List;

public class PetMapper implements Mapper<Pet, PetDto> {
    @Override
    public Pet toEntity(PetDto dto) {
        Pet pet = new Pet();
        pet.setId(dto.id());
        pet.setName(dto.name());
        pet.setBirthday(dto.birthday());
        pet.setColor(dto.color());

        Owner owner = new Owner(dto.owner().name(), dto.owner().birthday());
        owner.setId(dto.owner().id());

        pet.setOwner(owner);

        return pet;
    }

    @Override
    public PetDto toDto(Pet entity) {
        OwnerSimpleDto owner = new OwnerSimpleDto(entity.getOwner().getId(),
                entity.getOwner().getName(), entity.getOwner().getBirthday());
        List<PetSimpleDto> friends = new ArrayList<>();

        for (Pet pet : entity.getFriends()) {
            friends.add(new PetSimpleDto(pet.getId(), pet.getName(), pet.getBirthday(), pet.getColor()));
        }

        return new PetDto(entity.getId(), entity.getName(), entity.getBirthday(), entity.getColor(), owner, friends);
    }
}
